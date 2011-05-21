


require "Macaroni.Model.NodeList";
require "Macaroni.Model.AttributeTable"
require "Macaroni.Model.AttributeValue";
require "Macaroni.Model.Axiom";
Block = require "Macaroni.Model.Block";
require "Macaroni.Model.Cpp.Class";
require "Cpp/Common";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Cpp.Function";
require "Macaroni.Model.Cpp.FunctionOverload";
require "LuaGlue/LuaGlueCppFile";
require "LuaGlue/LuaGlueHFile";
require "Macaroni.Model.Library";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";
require "Log";

Axiom = Macaroni.Model.Axiom;
Class = Macaroni.Model.Cpp.Class;
Context = Macaroni.Model.Context;
FileName = Macaroni.Model.FileName;
Function = Macaroni.Model.Cpp.Function;
FunctionOverload = Macaroni.Model.Cpp.FunctionOverload;
Member = Macaroni.Model.Member;
NodeList = Macaroni.Model.NodeList;
Reason = Macaroni.Model.Reason;
Source = Macaroni.Model.Source;
Type = Macaroni.Model.Type;
Variable = Macaroni.Model.Cpp.Variable;

-- For obvious reasons, don't put any spaces or junk inside of the
-- string below
NEW_LINE = "\n";

local check = function(con, msg)
	if (not con) then
		error(msg, 3);
	end
end

local check2 = function(con, msg)
	if (not con) then
		error(msg, 4);
	end
end


LuaGlueGenerator = 
{
	new = function(rootNode) 
		check(rootNode ~= nil, 'Argument 1, "rootNode", must be specified.');
        local self = {}
        setmetatable(self, {["__index"] = LuaGlueGenerator});                                 
        self.LuaClass = "Macaroni::Lua::LuaClass";        
        self.RootNode = rootNode;
        self.LuaClassNode = self.RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
        self.Creators = self:CreatorsClass();
        self.CurrentFileName = FileName.Create("LuaGlue.lua");  
        self.lua_StateNode = self.RootNode:Find("lua_State");
        if (self.lua_StateNode == nil) then
			error('To use this generator, node "lua_State" must not be hollow.', 2);
        end 
        self.luaL_RegNode = self.RootNode:Find("luaL_Reg");
        if (self.luaL_RegNode == nil) then
			error('To use this generator, node "luaL_Reg" must not be hollow.', 2);
        end             
        return self;
    end,    
    
	LuaGlueAxioms =
	{
		LuaClassStart = Axiom.LuaCreate("LuaGlue.LuaClassWraps");
	},	

	RootNode = nil, -- This will be set by the Generate function.

	CreatorsClass = function(self)
		check(self ~= nil, "Member function called without self.");
		check(self.RootNode ~= nil, "Self must contain defined RootNode.");
		local me = {};
		me.primitives = self.RootNode:FindOrCreate("{C++ Primitives}");
		me.boolNode = me.primitives:FindOrCreate("bool");
		me.intNode = me.primitives:FindOrCreate("int");
		me.voidNode = me.primitives:FindOrCreate("void");
		me.stringNode = self.RootNode:FindOrCreate("std::string");
		me.charNode = me.primitives:Find("char");
		me.debugMetaTable = function(obj) 
			local mt = getmetatable(obj);
			log:Write("The meta table of obj is " .. tostring(mt) .. "... !");
			for i, v in ipairs(mt) do 
				log:Write("mt... " .. tostring(i) .. "=" .. tostring(v) .. ""); 
			end
		end;
		return me;
	end,
	
	Creators = nil,
	
	LuaWrapperArguments = function(self, node)
		log:Write("Creating wrapper args for " .. tostring(node.FullName) .. ".");
		local luaClass = "Macaroni::Lua::LuaClass";--RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
		local referenceTypeNode = "Macaroni::Lua::LuaClass";
		local rtn = {};	
		rtn.node = node;
		local attr = node.Attributes[luaClass];
		log:Write("Hope there ain't no errors.");
		if (attr == nil) then
			error("The given node " .. tostring(node.Fullname) ..
				" does not have an attribute " .. luaClass .. 
				" necessary to be wrapped in LuaGlue.", 2);
		end	
		if (not attr.IsTable) then
			error("The given node " .. tostring(node.Fullname) ..
				" has an attribute " .. luaClass .. 
				" but it is not a table.", 2);	
		end
		local args = attr.ValueAsTable;
		log:Write("args looks like dis: " .. tostring(args) .. "");
		local refTypeAttrValue = args["ReferenceType"];
		if (refTypeAttrValue == nil) then
			error('While wrapping node ' .. tostring(node.FullName) ..
				': no "ReferenceType" found.  This value must specify the type ' ..
				'used to store references to this in Lua.');
		end
		if (refTypeAttrValue.IsNode == false) then
			error('While wrapping node ' .. tostring(node.FullName) ..
				': "ReferenceType" must refer to a Node.');				
		end
		rtn.referenceType = refTypeAttrValue.ValueAsNode;
		rtn.metaNode = self.RootNode:FindOrCreate(rtn.node.FullName .. "LuaMetaData");
		return rtn;	
	end,
	
	-- For a given node, determines the methods used to manipulate it
	-- TODO change this to allow for depencency injection
	TypeManipulators = function(self, type)	
		local node = type.Node;
		local rtn = {};
		local success, args = pcall(function() return self:LuaWrapperArguments(node) end);
		if (success) then	
			rtn.referenceType = args.referenceType;	
			rtn.get = function(var, index) 
				return args.referenceType.FullName .. " & " .. var .. " = "  ..
					args.metaNode.FullName .. "::GetInstance(L, " .. index .. ");"; 
			end;
			rtn.convertArgument = function(var, index)
				local str = rtn.get(var .. "_AsRef", index);
				str = str .. NEW_LINE;
				if (type.Const) then
					str = str .. "const ";
				end
				str = str .. args.node.FullName .. " ";
				if (type.Pointer) then
					str = str .. "* ";
					if (type.ConstPointer) then
						str = str .. "const ";
					end
				else
					str = str .. "& ";				
				end
				str = str .. var .. " = ";
				if (type.Pointer == false) then
					str = str .. "*(";
				end
				str = str .. "*(" .. var .. "_AsRef)";
				if (type.Pointer == false) then
					str = str .. ")";
				end
				str = str .. ";";
				return str;
			end;
			rtn.put = function(var) return args.metaNode.FullName .. "::PutInstanceOnStack(L, " .. var .. ");"; end;			
		else
			rtn.referenceType = args.node;
			if (node == self.Creators.stringNode) then
				rtn.get = function(var, index)
					return "const std::string " .. var .. "(luaL_checkstring(L, " .. index .. "));"; 
				end;				
				rtn.put = function(var) return "lua_pushlstring(L, " .. var .. ".c_str(), " .. var .. ".length());" end;
			elseif (node.FullName == self.Creators.intNode.FullName) then
				rtn.get = function(var, index)
					return "int " .. var .. "(luaL_checkint(L, " .. index .. "));"; 
				end;
				rtn.put = function(var) return "lua_pushinteger(L, " .. var .. ");" end;
			else
				error('The given type "' .. tostring(type) .. '" cannot be manipulated.', 2);
			end
			rtn.convertArgument = rtn.get;
		end		
		return rtn;
	end,
	
		
	markNodeWithLua = function(self, node)
		local nodeInfo = NodeInfoList[node]
		nodeInfo.luaDef = "#include <" .. node:GetPrettyFullName("/") .. "Lua.h>\n";
	end,

	parseNode = function(self, rootPath, node, ptrNode)
		local hGen = LuaGlueHFile.new({
			node = node,
			ptrNode = ptrNode,
			path = rootPath:NewPath("/" .. node:GetPrettyFullName("/") .. "Lua.h");
		});        
		hGen:parse();
		local cppGen = LuaGlueCppFile.new({
			node = node,
			ptrNode = ptrNode,
			path = rootPath:NewPath("/" .. node:GetPrettyFullName("/") .. "Lua.cpp");
		});        
		cppGen:parse();
	end,

	findAllAttr = function(self, node)
		check(self ~= nil, "Function must be called with self.");		
		if (node == nil) then			
			return {};
		end    
		local rtn = {}
		local nodes = node.Children
		for i = 1, #nodes do
			local n = nodes[i];
			if (n.Attributes[self.LuaClass] ~= nil) then
				rtn[#rtn + 1] = n
			end
			for k,v in pairs(self:findAllAttr(n)) do 
				rtn[k] = v 
			end
		end
		return rtn;
	end,

	-- Finds all methods in a node which are not attributed as Properties
	findAllFunctionsInNode = function(self, node)
		if (node == nil) then
			error("Node cannot be nil.", 2);
		end
		local rtn = {};
		for i = 1, #node.Children do
			local child = node.Children[i];
			if (child.Member.TypeName == "Function") then
				for j = 1, #child.Children do
					local fon = child.Children[j]; -- FunctionOverload node
					if (fon.Member ~= nil and fon.Member.TypeName == 'FunctionOverload') then
						rtn[#rtn + 1] = fon;
					end
				end	
			end
		end
		return rtn;
	end,

	-- Returns a table with info on the method
	wrapMethod = function(self, methodOverloadNode) 
		check(self ~= nil, 'Argument "self" missing!');
		check(methodOverloadNode ~= nil, 'Argument "methodNode" missing!');
		check(methodOverloadNode.Member ~= nil, 'Argument "methodNode" has no Member defined!');		
		check(methodOverloadNode.Member.TypeName == "FunctionOverload", 'Argument "methodNode" must have Member defined as FunctionOverload.');
		local node = methodOverloadNode.Node.Node;
		check(node ~= nil, "The methodNode had no parent?! How can that be?!");
		local rtn = {}
		local methodName = methodOverloadNode.Node.Name
		rtn.name = methodName;
		local t = { }
		t[#t + 1]  = "static int " .. methodName .. "(lua_State * L)";		
		t[#t + 1] = "{";
		local startIndex;
		if (not methodOverloadNode.Member.Static) then
			startIndex = 2;
		else
			startIndex = 1;
		end		
		if (not methodOverloadNode.Member.Static) then
			local instanceType = Type.New(node, {});
			local tm = self:TypeManipulators(instanceType);
			t[#t + 1] = "\t" .. tm.get("instance", 1);
		end
		local argIndex = startIndex;
		for j = 1, #methodOverloadNode.Children do
			local nodeChild = methodOverloadNode.Children[j];
			local var = nodeChild.Member;
			if (var ~= nil and var.TypeName == "Variable") then
				local varType = var.Type;
				local varTm = self:TypeManipulators(varType);
				local argName = tostring(argIndex - (startIndex - 1));
				t[#t + 1] = "\t" .. varTm.convertArgument("arg" .. argName, argIndex);
				argIndex = argIndex + 1;
			end
		end
		local returnType = methodOverloadNode.Member.ReturnType;
		local returnTypeTm;
		if (returnType.Node.FullName ~= self.Creators.voidNode.FullName) then
			returnTypeTm = self:TypeManipulators(returnType);
		else
			returnTypeTm = null;
		end
		
		local methodCall = "";
		if (returnType.Node.FullName ~= self.Creators.voidNode.FullName) then		
			methodCall = methodCall .. tostring(returnType) .. " rtn = ";
		end		
		if (not methodOverloadNode.Member.Static) then
			methodCall = methodCall .. "instance->";
		else
			methodCall = methodCall .. self.node.FullName .. "::";
		end
		methodCall = methodCall .. methodName .. "(";
				
		local argIndexK = 1;
		for k = 1, #methodOverloadNode.Children do
			local nodeChild = methodOverloadNode.Children[k];
			local var = nodeChild.Member;
			if (var ~= nil and var.TypeName == "Variable") then
				methodCall = methodCall .. "arg" .. tostring(argIndexK);
				argIndexK = argIndexK + 1;
				if (k < #methodOverloadNode.Children) then
					methodCall = methodCall .. ", ";
				end	
			end		
		end
		methodCall = methodCall .. ");";
		t[#t + 1] = "\t" .. methodCall;
		if (returnType.Node.FullName == self.Creators.voidNode.FullName) then		
			t[#t + 1] = "\t" .. "return 0;";
		else			
			t[#t + 1] = "\t" .. returnTypeTm.put("rtn");
			t[#t + 1] = "\t" .. "return 1;";
		end	
		t[#t + 1] = "}";
		rtn.text = table.concat(t, "\n\t");
		return rtn;
	end,
	
	checkArgumentIsNodeGeneratorType = function(self, name, args)
		check2(args ~= nil, 'Argument "' .. name .. '" may not be nil.');
		check2(args.helperName ~= nil, 'Argument "' .. name .. '.helperName" may not be nil.');
		check2(args.originalNode ~= nil, 'Argument "' .. name .. '.originalNode" may not be nil.');
		check2(args.referenceType ~= nil, 'Argument "' .. name .. '.referenceType" may not be nil.');					
	end,
	
	wrapMethods = function(self, args)
		self:checkArgumentIsNodeGeneratorType("args", args);
				
		local t = { }				
		
		local funcs = self:findAllFunctionsInNode(args.originalNode);	
		t[#t + 1] = "struct " .. args.helperName .. "";
		t[#t + 1] = "{";
		t[#t + 1] = self:createHelperGc(args);
		t[#t + 1] = self:createHelperIndex(args);
		for i = 1, #funcs do
			local node = funcs[i];
			local methodInfo = self:wrapMethod(node);
			t[#t + 1] = methodInfo.text;		
		end
		t[#t + 1] = "};";
		return table.concat(t, "\n\t");	
	end,

	createHelperGc = function(self, args)
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __luaGc(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\t" .. args.referenceType.FullName .. " & ptr = " .. metaNodeName ..  "::GetInstance(L, 1);";
		t[#t + 1] = "\t\tptr.~" .. args.referenceType.Name .. "();";
		t[#t + 1] = "\t\treturn 0;"
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,	
	
	-- TODO: Move this into the ClassWrapper
	createHelperIndex = function(self, args)
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __index(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\t" .. args.referenceType.FullName .. " & ptr = " .. metaNodeName ..  "::GetInstance(L, 1);";
		t[#t + 1] = "\t\tstd::string index(luaL_checkstring(L, 2));";
		t[#t + 1] = "\t\treturn " .. metaNodeName .. "::Index(L, ptr, index);"
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,	
	
	getMetaNode = function(self, node)
		local metaNode = self.RootNode:FindOrCreate(node.FullName .. "LuaMetaData");
		return metaNode;
	end,
	
	ClassWrapper = { -- like a nested class, this has the helper methods called by Lua
		new = function(parent, node, referenceType)
			check(parent ~= nil, "Parent instance must be passed to ClassWrapper.");
			check(node ~= nil, 'Argument "node" may not be nil.');	
			check(node.Member ~= nil, 'Argument "node.Member" may not be nil.');	
			local self = {}
			setmetatable(self, {["__index"] = parent.ClassWrapper});         
			self.helperName = node.Name .. "MethodLuaGlue";		
			self.creators = parent:CreatorsClass();
			self.originalNode = node;
			-- rtn.node = rtn.originalNode;
			self.globalTableName = self.originalNode:GetPrettyFullName(".");
			self.lua_StateNode = parent.lua_StateNode;
			self.luaL_RegNode = parent.luaL_RegNode;
			self.metaTableName = self.originalNode:GetPrettyFullName(".");	
			self.metaNode = parent:getMetaNode(self.originalNode);
			self.axiom = parent.LuaGlueAxioms.LuaClassStart;
			self.referenceType = referenceType; 
			self.src = Source.Create(parent.CurrentFileName, 3, 5);
			self.reason = Reason.Create(self.axiom, node.Member.ReasonCreated.Source);
			self.parent = parent;
			return self;			
		end,				
		
		blocks = function(self)
			local blockHome = self.metaNode:FindOrCreate("functionPtrStructBlock");
			log:Write("Going to create block for function pointer struct.");
			local helperGc = self.parent:createHelperGc(self);
			local helperIndex = self.parent:createHelperIndex(self);
			local wrappedFunctions = self.parent:wrapMethods(self);
			local memberText = {};
			local staticText = {};
			local funcs = self.parent:findAllFunctionsInNode(self.originalNode);
			for i = 1, #funcs do
				local fNode = funcs[i]
				local tableEntry = '{"' .. fNode.Node.Name .. '", ' 
					.. self.helperName .. "::" .. fNode.Node.Name .. '}'; 
				if (not fNode.Member.Static) then
					staticText[#staticText + 1] = tableEntry;
				else
					memberText[#memberText + 1] = tableEntry;
				end
			end
			log:Write("Creating me a block.");
			local block = Block.Create(blockHome, "cpp",
[[
namespace
{	
	]] .. wrappedFunctions .. [[
	
	
	static const struct luaL_Reg globalTableMethods[]=
	{
		]] .. self:luaLRegConcat(staticText, ",\n\t")	.. [[
		//{"SplitComplexName", NodeLuaFunctions::SplitComplexName},
		{0, 0}
	};

	static const struct luaL_Reg metaTableMethods[]=
	{
	]] .. self:luaLRegConcat(memberText, ",\n\t")	.. [[
		//{"__eq", NodeLuaFunctions::__eq},
		{"__gc", ]] .. self.helperName .. [[::__luaGc},
		{"__index", ]] .. self.helperName .. [[::__index},
		//{"__tostring", NodeLuaFunctions::__tostring},
		{0, 0}
	};	
}; // end anon-namespace
]], self.reason);						
		end,
		
		createImport = function(self, node, path)
			local imports = NodeList.New({});   
			local nodeClass = Class.Create(CurrentLibrary, node, imports, self.reason);
			local fileName = FileName.Create(path);
			node:SetHFilePath(self.reason, fileName);
		end,
	    
		createLuaImports = function(self)
			-- TODO: Review. This should not be needed... if lua_State * L is 
			-- added anywhere, the magic of the C++ generators should 
			-- automatically add the import.
			-- self:createImport(self.lua_StateNode, self.parent.luaImportCode);
			-- self:createImport(self.luaL_RegNode, self.parent.luaImportCode);
			
			--TODO: I'm putting the imports on the class in "createClass"
			-- consider deleting this function.
		end,
		
		createClass = function(self)        
			log:Write("Wrapping " .. tostring(self.originalNode) .. " as " .. tostring(self.metaNode) .. ".");			
			local imports = NodeList.New({
				self.lua_StateNode,
				self.luaL_RegNode,
				self.parent.RootNode:FindOrCreate("std::string")
			});        
			for i = 1, #imports do
				log:Write("imports[".. i .. "]=".. tostring(imports[i]));
			end
			-- Mix all imports of the wrapped node with this one.
			local otherImports = self.originalNode.Member.ImportedNodes;
			for i = 1, #otherImports do
				local otherImport = otherImports[i];
				imports[#imports + 1] = otherImport;
			end
			
			local metaClass = Class.Create(CurrentLibrary, self.metaNode, imports, self.reason);    			    
			-- static bool IsType(lua_State * L, int index);        
		end,
		
		createIndexMethodBody = function(self)
			local funcs = self.parent:findAllFunctionsInNode(self.originalNode);
			local first = true;				
			local t = {}
			for i = 1, #funcs do
				local node = funcs[i];
				if (first) then
					t[#t + 1] = 'if (index == "' .. node.Node.Name ..'")';
					first = false;
				else
					t[#t + 1] = 'else if (index == "' .. node.Node.Name ..'")';
				end
				t[#t + 1] = '{';
				t[#t + 1] = '\tlua_pushcfunction(L, ' .. self.helperName .. '::' .. node.Node.Name .. ');';
				t[#t + 1] = '}';					
			end
			t[#t + 1] = 'else'
			t[#t + 1] = '{'
			t[#t + 1] = "\tlua_pushnil(L);"
			t[#t + 1] = '}'	
			t[#t + 1] = 'return 1;'	
			
			return table.concat(t, "\n\t");		
		end,
		
		createIndexMethod = function(self)			
			local node = self.metaNode:FindOrCreate("Index");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);		
			local rtnType = Type.New(self.parent.Creators.intNode, { });
			log:Write("Going to create Index.");
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
									   rtnType,
									   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);
			local arg2 = fo1.Node:FindOrCreate("ptr");
			local arg2Type = Type.New(self.referenceType, { Reference = true });
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);		
			local arg3 = fo1.Node:FindOrCreate("index");			
			local arg3Type = Type.New(self.parent.Creators.stringNode, { Const = true, Reference = true });
			Variable.Create(arg3, "Access_Public", false, arg3Type, "", self.reason);		
							
			local methodBody = self:createIndexMethodBody();						
			fo1:SetCodeBlock(methodBody, self.src);				
			
		end,
	    
		createIsType = function(self)
			local node = self.metaNode:FindOrCreate("IsType");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
			log:Write("Going to create IsType.");
			local rtnType = Type.New(self.parent.Creators.boolNode, { Pointer = false });
			log:Write("Going to create IsType.");
			local func = Function.Create(node, self.reason);
			local fo = FunctionOverload.Create(func, false, "Access_Public", true, 
										   rtnType,
										   false, self.reason);			
			func = node.Member;
			local arg1 = fo.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);
			local arg2 = fo.Node:FindOrCreate("index");			
			local arg2Type = Type.New(self.parent.Creators.intNode, {});
			local var = Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);		
							
			
			fo:SetCodeBlock(
[[
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != 0) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, "]] .. self.metaTableName .. [[");
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
]], self.src);
		end,
	    
		createOpenInLua = function(self)
			local node = self.metaNode:FindOrCreate("OpenInLua");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
			log:Write("Going to create IsType.");
			local rtnType = Type.New(self.parent.Creators.intNode, { });
			log:Write("Going to create IsType.");
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
										   rtnType,
										   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);					
			
			fo1:SetCodeBlock(
[[
	luaL_getmetatable(L, "]] .. self.metaTableName .. [[");
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, "]] .. self.metaTableName .. [["); // create metaTable
	luaL_register(L, 0, metaTableMethods);
	// TODO: Put dependencies here		
	luaL_register(L, "]] .. self.globalTableName .. [[", globalTableMethods);		
	return 1;
]], self.src);
		end,

		createGetInstance = function(self)
			local node = self.metaNode:FindOrCreate("GetInstance");
			log:Write("Going to create GetInstance.");
			local rtnType = Type.New(self.referenceType, { Reference = true });
			log:Write("Going to create IsType.");		
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
										   rtnType,
										   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			log:Write("GETINSTANCE 2");
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);					
			local arg2 = fo1.Node:FindOrCreate("index");	
			log:Write("H+RMMMR?");	
			local arg2Type = Type.New(self.parent.Creators.intNode, { });
			log:Write("WHHH!?")
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);					
			log:Write("WHAT?!!");
			local refTypeStr = self.referenceType.FullName;
			fo1:SetCodeBlock(
[[
	]] .. refTypeStr .. [[ * ptrToPtr = (]] .. refTypeStr .. [[ *) luaL_checkudata(L, index, "]] .. self.metaTableName .. [[");
	]] .. refTypeStr .. [[ & ptr = dynamic_cast<]] .. refTypeStr .. [[ &>(*ptrToPtr);
	return ptr;
]], self.src);
		end,
		
		createPutInstanceOnStack = function(self)
			local node = self.metaNode:FindOrCreate("PutInstanceOnStack");
			log:Write("Going to create PutInstanceOnStack.");
			local rtnType = Type.New(self.parent.Creators.voidNode, { });
			log:Write("Going to create putInstanceOnStack.");		
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
										   rtnType,
										   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });		
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);					
			
			local arg2 = fo1.Node:FindOrCreate("ptr");	
			local arg2Type = Type.New(self.referenceType, { Const = true, Reference = true });
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);					
			
			local refTypeStr = self.referenceType.FullName;
			fo1:SetCodeBlock(
[[
	if (!ptr) 
	{
		lua_pushnil(L);
	}
	else
	{
		void * memory = lua_newuserdata(L, sizeof(]] .. refTypeStr .. [[));
		]] .. refTypeStr .. [[ * instance = new (memory) ]] .. refTypeStr .. [[();		
		(*instance).operator=(ptr);		
		luaL_getmetatable(L, "]] .. self.metaTableName .. [[");
		lua_setmetatable(L, -2); 
	}	
]], self.src);
		end,
		
		luaLRegConcat = function(self, t, seperator)
			if (#t < 1) then
				return "";
			else
				return table.concat(t, seperator) .. ',';
			end						
		end,
		
		wrapClass = function(self)
			self:blocks();
			self:createLuaImports();
			self:createClass();
			self:createIsType(); 
			self:createOpenInLua();
			self:createGetInstance();
			self:createPutInstanceOnStack();
			self:createIndexMethod();
		    
			log:Write("End of wrapClassFunction.");
		end
	}, -- End object "ClassWrapper"		
	
	wrapClass = function(self, args)		
		check(args ~= nil, 'Argument "args" may not be nil.');
		check(args.node ~= nil, 'Argument "args.node" may not be nil.');
		local wrapper = self.ClassWrapper.new(self, args.node, args.referenceType);
		wrapper:wrapClass();
	end
	
}; -- end LuaGlueGenerator class




-- To have Lua glue autogenerated, each Class must have a reference type,
-- such as a boost smartptr.

function Generate(library, path, arguments)	
	log.Init("LuaGlue");
	log = {
		Write = function(self, msg)
			print("[LUA]:" .. msg);
		end
	}
	log:Write("Entered Generate");
	arguments = arguments or {}		
    CurrentLibrary = library;
    local generator = LuaGlueGenerator.new(library.Context.Root);
    --RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
    
    local classes = generator:findAllAttr(generator.RootNode);
    
    log:Write("Found " .. #(classes) .. " classes with attributes...");
    log:Write("BEGIN LUA GLUE");
    for i, class in ipairs(classes) do
		local wrapArgs = generator:LuaWrapperArguments(class);
		log:Write("MIDDLE ...");
        generator:wrapClass(wrapArgs)
    end
    --_.each(classes, wrapClass);
    log:Write("END LUA GLUE");
    --[[markNodeWithLua(library.Context.Root:Find("Macaroni::Model::Cpp::Access"));
    markNodeWithLua(library.Context.Root:Find("Macaroni::Model::Cpp::Class"));
    markNodeWithLua(library.Context.Root:Find("Macaroni::Model::Node"));
    
    local ClassParent = library.Context.Root:Find("Macaroni::Model::Cpp::ClassParent"); 
    local ClassParentPtr = library.Context.Root:Find("Macaroni::Model::Cpp::ClassParentPtr");
    parseNode(path, ClassParent, ClassParentPtr);]]--
    
end



--if Test == nil then 
--	Test = { register = function() end }
--end

