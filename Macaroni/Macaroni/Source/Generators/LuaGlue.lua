require "Macaroni.Model.AttributeTable"
require "Macaroni.Model.AttributeValue";
require "Macaroni.Model.Axiom";
Block = require "Macaroni.Model.Block";
require "Macaroni.Model.Cpp.Class";
require "Cpp/Common";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Cpp.Function";
require "LuaGlue/LuaGlueCppFile";
require "LuaGlue/LuaGlueHFile";
require "Macaroni.Model.Library";
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";

Axiom = Macaroni.Model.Axiom;
Class = Macaroni.Model.Cpp.Class;
Context = Macaroni.Model.Context;
FileName = Macaroni.Model.FileName;
Function = Macaroni.Model.Cpp.Function;
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

log = function(msg)
	print("[LuaGlue.lua]:" .. msg);
end

log("Parsing file.");

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
			log("The meta table of obj is " .. tostring(mt) .. "... !");
			for i, v in ipairs(mt) do 
				log("mt... " .. tostring(i) .. "=" .. tostring(v) .. ""); 
			end
		end;
		return me;
	end,
	
	Creators = nil,
	
	LuaWrapperArguments = function(self, node)
		log("Creating wrapper args for " .. tostring(node.FullName) .. ".");
		local luaClass = "Macaroni::Lua::LuaClass";--RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
		local referenceTypeNode = "Macaroni::Lua::LuaClass";
		local rtn = {};	
		rtn.node = node;
		local attr = node.Attributes[luaClass];
		log("Hope there ain't no errors.");
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
		log("args looks like dis: " .. tostring(args) .. "");
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
				rtn.put = function(var) return "luaL_pushstring(L, " .. var .. ".c_str());" end;
			elseif (node.FullName == self.Creators.intNode.FullName) then
				rtn.get = function(var, index)
					return "int " .. var .. "(luaL_checkint(L, " .. index .. "));"; 
				end;
				rtn.put = function(var) return "lua_pushint(L, " .. var .. ");" end;
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
				rtn[#rtn + 1] = child;			
			end
		end
		return rtn;
	end,

	-- Returns a table with info on the method
	wrapMethod = function(self, methodNode) 
		check(self ~= nil, 'Argument "self" missing!');
		check(methodNode ~= nil, 'Argument "methodNode" missing!');
		check(methodNode.Member ~= nil, 'Argument "methodNode" has no Member defined!');
		check(methodNode.Member.TypeName == "Function", 'Argument "methodNode" must have Member defined as Function.');
		local node = methodNode.Node;
		check(node ~= nil, "The methodNode had no parent?! How can that be?!");
		local rtn = {}
		rtn.name = methodNode.Name;
		local t = { }
		t[#t + 1]  = "int " .. methodNode.Name .. "(lua_State * L)";		
		t[#t + 1] = "{";
		local startIndex;
		if (not methodNode.Member.Static) then
			startIndex = 2;
		else
			startIndex = 1;
		end		
		if (not methodNode.Member.Static) then
			local instanceType = Type.New(node, {});
			local tm = self:TypeManipulators(instanceType);
			t[#t + 1] = "\t" .. tm.get("instance", 1);
		end
		local argIndex = startIndex;
		for j = 1, #methodNode.Children do
			local nodeChild = methodNode.Children[j];
			local var = nodeChild.Member;
			if (var ~= nil and var.TypeName == "Variable") then
				local varType = var.Type;
				local varTm = self:TypeManipulators(varType);
				local argName = tostring(argIndex - (startIndex - 1));
				t[#t + 1] = "\t" .. varTm.convertArgument("arg" .. argName, argIndex);
				argIndex = argIndex + 1;
			end
		end
		local returnType = methodNode.Member.ReturnType;
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
		if (not methodNode.Member.Static) then
			methodCall = methodCall .. "instance->";
		else
			methodCall = methodCall .. self.node.FullName .. "::";
		end
		methodCall = methodCall .. methodNode.Name .. "(";
				
		local argIndexK = 1;
		for k = 1, #methodNode.Children do
			local nodeChild = methodNode.Children[k];
			local var = nodeChild.Member;
			if (var ~= nil and var.TypeName == "Variable") then
				methodCall = methodCall .. "arg" .. tostring(argIndexK);
				argIndexK = argIndexK + 1;
				if (k < #methodNode.Children) then
					methodCall = methodCall .. ", ";
				end	
			end		
		end
		methodCall = methodCall .. ");";
		t[#t + 1] = "\t" .. methodCall;
		if (returnType.Node.FullName == self.Creators.voidNode.FullName) then		
			t[#t + 1] = "\t" .. "return 0";
		else			
			t[#t + 1] = "\t" .. returnTypeTm.put("rtn");
			t[#t + 1] = "\t" .. "return 1;";
		end	
		t[#t + 1] = "}";
		rtn.text = table.concat(t, "\n\t");
		return rtn;
	end,
	
	wrapMethods = function(self, args)
		check(args ~= nil, 'Argument "args" may not be nil.');
		check(args.node ~= nil, 'Argument "args.node" may not be nil.');
				
		local t = { }				
		
		local funcs = self:findAllFunctionsInNode(args.node);	
		t[#t + 1] = "struct WrappedFunctions";
		t[#t + 1] = "{";
		for i = 1, #funcs do
			local node = funcs[i];
			local methodInfo = self:wrapMethod(node);
			t[#t + 1] = methodInfo.text;		
		end
		t[#t + 1] = "}";
		return table.concat(t, "\n\t");	
	end,

	wrapClass = function(self, args)
		check(args ~= nil, 'Argument "args" may not be nil.');
		check(args.node ~= nil, 'Argument "args.node" may not be nil.');
		
		log("wrapClass enter");
		local Creators = self:CreatorsClass();
		local originalNode = args.node;
		local globalTableName = originalNode:GetPrettyFullName(".");
		local lua_StateNode = self.RootNode:FindOrCreate("lua_State");
		local metaTableName = originalNode:GetPrettyFullName(".");	
		local metaNode = self.RootNode:FindOrCreate(originalNode.FullName .. "LuaMetaData");
		local axiom = self.LuaGlueAxioms.LuaClassStart;
		local referenceType = args.referenceType; 
		local src = Source.Create(self.CurrentFileName, 3, 5);
		local reason = Reason.Create(axiom, src);
	    
	    
	    
		function blocks()
			local blockHome = metaNode:FindOrCreate("functionPtrStructBlock");
			log "Going to create block for function pointer struct.";
			local wrappedFunctions = self:wrapMethods(args);
			local memberText = {};
			local staticText = {};
			local funcs = self:findAllFunctionsInNode(args.node);
			for i = 1, #funcs do
				local fNode = funcs[i]
				if (not fNode.Member.Static) then
					staticText[#staticText] = fNode.Name;
				else
					memberText[#memberText] = fNode.Name;
				end
			end
			log "Creating me a block.";
			local block = Block.Create(blockHome, "cpp",
[[
namespace
{
	static const struct luaL_Reg globalTableMethods[]=
	{
		]] .. table.concat(staticText, "\n\t")	.. [[
		//{"SplitComplexName", NodeLuaFunctions::SplitComplexName},
		{nullptr, nullptr}
	};

	static const struct luaL_Reg metaTableMethods[]=
	{
	]] .. table.concat(memberText, "\n\t")	.. [[
		//{"__eq", NodeLuaFunctions::__eq},
		//{"__gc", NodeLuaFunctions::luaGc},
		//{"__index", NodeLuaFunctions::__index},
		//{"__tostring", NodeLuaFunctions::__tostring},
		{nullptr, nullptr}
	};	
}; // end anon-namespace
]], reason);						
		end
	    
		function createImport(node, path)
			local imports = NodeList.New({});   
			local nodeClass = Class.Create(CurrentLibrary, node, imports, reason);
			local fileName = FileName.Create(path);
			node:SetHFilePath(reason, fileName);
		end
	    
		function createLuaImports()
			createImport(lua_StateNode, '"lua/luaCpp.h"');
		end
	    
		function createClass()        
			log("Wrapping " .. tostring(originalNode) .. " as " .. tostring(metaNode) .. ".");
			local imports = NodeList.New({});        
			local metaClass = Class.Create(CurrentLibrary, metaNode, imports, reason);        
			-- static bool IsType(lua_State * L, int index);        
		end
	    
		function createIsType()
			local node = metaNode:FindOrCreate("IsType");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
			log "Going to create IsType.";
			local rtnType = Type.New(Creators.boolNode, { Pointer = true });
			log "Going to create IsType.";
			local func = Function.Create(node, false, "Access_Public", true, 
										   rtnType,
										   false, reason);
			func = node.Member;		
			local arg1 = node:FindOrCreate("L");		
			local arg1Type = Type.New(lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", reason);
			local arg2 = node:FindOrCreate("index");			
			local arg2Type = Type.New(Creators.intNode, {});
			local var = Variable.Create(arg2, "Access_Public", false, arg2Type, "", reason);		
							
			
			func:SetCodeBlock(
[[
	// Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != nullptr) // is user data
	{
		// Compares metatable from user data to one in registry.
		if (lua_getmetatable(L, index))
		{
			lua_getfield(L, LUA_REGISTRYINDEX, "]] .. metaTableName .. [[");
			if (lua_rawequal(L, -1, -2))
			{
				returnValue = true;
			}
			lua_pop(L, 2); // remove meta tables
		}
	}
	return returnValue;
]], src);
		end
	    
		function createOpenInLua()
			local node = metaNode:FindOrCreate("OpenInLua");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
			log "Going to create IsType.";
			local rtnType = Type.New(Creators.intNode, { });
			log "Going to create IsType.";
			local func = Function.Create(node, false, "Access_Public", true, 
										   rtnType,
										   false, reason);
			func = node.Member;		
			local arg1 = node:FindOrCreate("L");		
			local arg1Type = Type.New(lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", reason);					
			
			func:SetCodeBlock(
[[
	luaL_getmetatable(L, "]] .. metaTableName .. [[");
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}
	luaL_newmetatable(L, "]] .. metaTableName .. [["); // create metaTable
	luaL_register(L, nullptr, metaTableMethods);
	// TODO: Put dependencies here		
	luaL_register(L, "]] .. globalTableName .. [[", globalTableMethods);		
	return 1;
]], src);
		end

		function createGetInstance()
			local node = metaNode:FindOrCreate("GetInstance");
			log "Going to create GetInstance.";
			local rtnType = Type.New(referenceType, { Reference = true });
			log "Going to create IsType.";		
			local func = Function.Create(node, false, "Access_Public", true, 
										   rtnType,
										   false, reason);
			func = node.Member;		
			local arg1 = node:FindOrCreate("L");		
			local arg1Type = Type.New(lua_StateNode, { Pointer = true });
			log "GETINSTANCE 2";
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", reason);					
			local arg2 = node:FindOrCreate("index");	
			log "H+RMMMR?";	
			local arg2Type = Type.New(Creators.intNode, { });
			log "WHHH!?"
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", reason);					
			log "WHAT?!!"
			local refTypeStr = referenceType.FullName;
			func:SetCodeBlock(
[[
	]] .. refTypeStr .. [[ * ptrToPtr = (]] .. refTypeStr .. [[ *) luaL_checkudata(L, index, "]] .. metaTableName .. [[");
	]] .. refTypeStr .. [[ & ptr = dynamic_cast<]] .. refTypeStr .. [[ &>(*ptrToPtr);
	return ptr;
]], src);
		end	
		
		function createPutInstanceOnStack()
			local node = metaNode:FindOrCreate("PutInstanceOnStack");
			log "Going to create PutInstanceOnStack.";
			local rtnType = Type.New(Creators.voidNode, { });
			log "Going to create putInstanceOnStack.";		
			local func = Function.Create(node, false, "Access_Public", true, 
										   rtnType,
										   false, reason);
			func = node.Member;		
			local arg1 = node:FindOrCreate("L");		
			local arg1Type = Type.New(lua_StateNode, { Pointer = true });		
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", reason);					
			
			local arg2 = node:FindOrCreate("ptr");	
			local arg2Type = Type.New(referenceType, { Const = true, Reference = true });
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", reason);					
			
			local refTypeStr = referenceType.FullName;
			func:SetCodeBlock(
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
		luaL_getmetatable(L, "]] .. metaTableName .. [[");
		lua_setmetatable(L, -2); 
	}	
]], src);
		end
		
		blocks();
		createLuaImports();
		createClass();
		createIsType(); 
		createOpenInLua();
		createGetInstance();
		createPutInstanceOnStack();
	    
		log "End of wrapClassFunction."
	end
}; -- end LuaGlueGenerator class




-- To have Lua glue autogenerated, each Class must have a reference type,
-- such as a boost smartptr.
function Generate(library, path)	
    CurrentLibrary = library;
    local generator = LuaGlueGenerator.new(library.Context.Root);
    --RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
    
    local classes = generator:findAllAttr(generator.RootNode);
    
    log("Found " .. #(classes) .. " classes with attributes...");
    log("BEGIN LUA GLUE");
    for i, class in ipairs(classes) do
		local wrapArgs = generator:LuaWrapperArguments(class);
		log("MIDDLE ...");
        generator:wrapClass(wrapArgs)
    end
    --_.each(classes, wrapClass);
    log "END LUA GLUE";
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

