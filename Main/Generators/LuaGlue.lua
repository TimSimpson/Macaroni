


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
        self.LuaFunction = "Macaroni::Lua::LuaFunction";
        self.LuaProperty = "Macaroni::Lua::LuaProperty";
        self.RootNode = rootNode;
        self.LuaClassNode = self.RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
        self.LuaPropGetNode = self.RootNode:FindOrCreate("Macaroni::Lua::LuaPropGet");
        self.LuaPropSetNode = self.RootNode:FindOrCreate("Macaroni::Lua::LuaPropSet");
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
	
	LuaWrapperArguments = function(self, node, lastCaller)
		-- Given a node, returns a table with info on how to wrap it.
		-- For example, this includes the node containing the LuaGlue class,
		-- the node with the reference type used for this node (assuming this
		-- node points to a type), etc.
		-- Throws errors if the Attributes on a Node don't match something this
		-- generator can use.
		if lastCaller == nil then
			lastCaller = {}
		else
			-- Check for cycles.
			for i = 1, #lastCaller do
				for j = i + 1, #lastCaller do
					if lastCaller[i].FullName == lastCaller[j].FullName then
						str = "There is a circular reference involving the " ..
						    "'SameAsNode' annotation.  Here's the call list:";
						for k = 1, #lastCaller do
							if k > 1 then
								str = str .. "->";
							end
							str = str .. "(" .. lastCaller[k].FullName .. ")";
						end
						error(str);
					end
				end
			end
		end
		log:Write("Creating wrapper args for " .. tostring(node.FullName) .. ".");
		local luaClass = "Macaroni::Lua::LuaClass";--RootNode:FindOrCreate("Macaroni::Lua::LuaClass");
		local referenceTypeNode = "Macaroni::Lua::LuaClass";
		local rtn = {};	
		rtn.node = node;
		local attr = node.Attributes[luaClass];
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
		
		-- If "SameAsNode" is set, it means another node has the values for
		-- this type as well and we can reuse those.
		local sameAsNode = args["SameAsNode"];
		if (sameAsNode ~= nil) then
			if not sameAsNode.IsNode then
				error('While wrapping node ' .. tostring(node.FullName) ..
				': "SameAsNode" must refer to a Node.');	
			end
			otherNode = sameAsNode.ValueAsNode;
			lastCaller[#lastCaller + 1] = node;
			wrapperArgs = self:LuaWrapperArguments(otherNode, lastCaller);
			wrapperArgs.glueAlreadyExists = true;
			wrapperArgs.node = node;
			rtn = wrapperArgs;
		else		
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
			rtn.glueAlreadyExists = false
			rtn.referenceType = refTypeAttrValue.ValueAsNode;		
			
			local existingMetaNode = args["LuaGlueClass"];
			log:Write("LuaGlueClass = " .. tostring(existingMetaNode));
			if (existingMetaNode == nil) then
				rtn.metaNode = self.RootNode:FindOrCreate(rtn.node.FullName .. "LuaMetaData");
			else
				if (existingMetaNode.IsNode == false) then
					error('While wrapping node ' .. tostring(node.FullName) ..
						  ': "LuaGlueClass" must refer to a Node.');
				end
				rtn.glueAlreadyExists = true
				rtn.metaNode = existingMetaNode.ValueAsNode;
			end
		end
		return rtn;	
	end,
		
	TypeManipulators = function(self, type)	
		-- Returns a table with methods to write wrapper code.
		--
		-- convertArgument - returns code which grabs a reference to the type
		--   from the Lua Stack, then converts it to a reference to the real
		--   type.  Example:
		--     convert("inst", 5) = 
		--     [[SomeClassPtr & inst_AsRef = SomeClassLuaMetaData::GetInstance(L, 5);
		--       const SomeClass & inst = *(inst_AsRef); ]]
		--
		-- "get" - code which, given a var and index, writes code to make a
		--   a variable named "var" as the reference type.  Example:
		--     get("ptr", 3) = 
		--     'SomeClassPtr & ptr = SomeClassLuaMetaData::GetInstance(L, 3)'		
		--
		-- "put" - code which given a var puts it into the Lua stack. Example:
		--     put("something") =
		--     'SomeClassLuaMetaData::PutInstanceOnStack(L, something);'
		-- 
		-- An important node is that if the type lacks special attributes, its
		-- ok as long as its some primitive type the code can guess.  So if its
 		-- a std::string, integer, boolean or something the code will be work.
		-- 
		local node = type.Node;
		local rtn = {};
		local success, args = pcall(function() return self:LuaWrapperArguments(node) end);
		if (success) then	
			-- imports = self.metaNode.Member.ImportedNodes;
			-- alreadyImported = false
			-- for i in #imports do
			-- 	if imports[i].FullName == args.metaNode.FullName then
			-- 		alreadyImported = true
			-- 		i = #imports + 1
			-- 	end
			-- end
			-- if not alreadyImported then
			-- 	imports[#imports + 1] = args.metaNode 
			-- end
			rtn.referenceType = args.referenceType;	
			rtn.get = function(var, index) 
				return args.referenceType.FullName .. " & " .. var .. " = "  ..
					args.metaNode.FullName .. "::GetInstance(L, " .. index .. ");"; 
			end;
			if args.node == args.referenceType then
				rtn.convertArgument = rtn.get;
			else
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
			end
			rtn.put = function(var) return args.metaNode.FullName .. "::PutInstanceOnStack(L, " .. var .. ");"; end;			
			
		else
			rtn.referenceType = node; -- args.node;			
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
			elseif (node.FullName == self.Creators.boolNode.FullName) then
				rtn.get = function(var, index)
					return "bool " .. var .. "((bool) luaL_checkboolean(L, " .. index .. "));"; 
				end;
				rtn.put = function(var) return "lua_pushboolean(L, (int)" .. var .. ");" end;
			else
				error('The given type "' .. tostring(type) 
				      .. '" cannot be manipulated. '
					  .. 'It is not a primitive and an attempt to get wrapper ' 
					  .. 'using annotation data resulted in this error:'
					  .. args, 2);
			end
			rtn.convertArgument = rtn.get;
		end		
		return rtn;
	end,
	
		
	markNodeWithLua = function(self, node)
		-- Adds the luaDef to the passed in Node.
		local nodeInfo = NodeInfoList[node]
		nodeInfo.luaDef = "#include <" .. node:GetPrettyFullName("/") .. "Lua.h>\n";
	end,

	parseNode = function(self, rootPath, node, ptrNode)
		-- !!! DEPRECATED !! Delete?
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
		-- Given a node, iterates recursively through NodeSpace and returns a
		-- a list of all Nodes which are marked with the "LuaClass" annotation.
		
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
	
	findAllFunctionsWithAttribute = function(self, node, attributeName, criteria)
		-- Iterates node (not recursively) and returns list of all function 
		-- overloads matching attributeName.
		
		if (node == nil) then
			error("Node cannot be nil.", 2);
		end
		local rtn = {};
		for i = 1, #node.Children do
			local child = node.Children[i];
			if (child.Member.TypeName == "Function") then
				for j = 1, #child.Children do
					local fon = child.Children[j]; -- FunctionOverload node
					if (fon.Member ~= nil and fon.Member.TypeName == 'FunctionOverload'
						and fon.Attributes[attributeName] ~= nil
						and criteria(fon)) then
						rtn[#rtn + 1] = fon;
					end
				end	
			end
		end
		return rtn;
	end,
	
	findAllFunctionsInNode = function(self, node, findStatic)
		-- Iterates node (not recursively) and returns list of all function 
		-- overloads.
		return self:findAllFunctionsWithAttribute(node, self.LuaFunction,
		           (function(node) 
						if findStatic == nil then
							return true;
						end
						return node.Member.Static == findStatic 
					end));
	end,	
	
	findAllGettersInNode = function(self, node, static)
		return self:findAllFunctionsWithAttribute(node, self.LuaProperty,
		   (function(node)
				varCount, returnType = self:getFunctionArgCountAndReturnType(node);
				if (varCount > 1) then
					error("The node " .. node.Node.FullName .. " is marked as "
					      .. "@Property but does not look like a getter or " 
					      .. "setter.");
				end
				if varCount == 1 then
					return false;
				end
				if (returnType.Node.FullName == self.Creators.voidNode.FullName) then
					error("The node " .. node.Node.FullName .. " is marked as "
					      .. "@Property, has zero arguments but does not "
					      .. "return anything, so it can't be a getter.");
				end
				return node.Member.Static == static;				
		   end));
	end,
	
	findAllSettersInNode = function(self, node)
		return self:findAllFunctionsWithAttribute(node, self.LuaProperty,
		   (function(node)
				varCount, returnType = self:getFunctionArgCountAndReturnType(node);
				if (varCount > 1) then
					error("The node " .. node.Node.FullName .. " is marked as "
					      .. "@Property but does not look like a getter or " 
					      .. "setter.");
				end
				if (varCount == 0) then
					if (returnType.Node.FullName 
					    == self.Creators.voidNode.FullName) then
						error("The node " .. node.Node.FullName .. " is marked as "
							  .. "@Property, has zero arguments but does not "
							  .. "return anything, so it can't be a getter.");
					end
					return false;
				end				
				-- Anything else is a setter, even if it returns the value.
				return true;
		   end));
	end,
	
	findFirstArgument = function(self, methodOverloadNode)
		-- Returns the first Variable in the node.
		for i=1, #methodOverloadNode.Children do
			local node = methodOverloadNode.Children[i];
			if node.Member ~= nil and node.Member.TypeName == "Variable" then
				return node;
			end
		end
	end,
	
	getFunctionArgCountAndReturnType = function(self, node)
		-- Returns the number of arguments in a function and its return type.
		local vars = node.Children
		local varCount = 0
		for i = 1, #vars do
			if (node.Children[i].Member.TypeName == "Variable") then
				varCount = varCount + 1;
			end
		end		
		local returnType = node.Member.ReturnType;				
		return varCount, returnType;
	end,
	
	wrapMethod = function(self, methodOverloadNode) 
		-- Returns a table with: 
		--   name - The name of the method which is wrapped.
		--   text - The actual code for the glue method which will call the
		--          wrapped method.
		-- Example: Let's say "SomeClass::ComputeSomething" has a signature as:
		--   int SomeClass::ComputeSomething(const SomeOtherClass & a1, int a2);
		-- Then "name" = ComputeSomething,
		-- text = [[
		--     SomeClassPtr instance = SomeClassLuaMetaData::GetInstance(L, 1);
		--     SomeOtherClassPtr arg1_AsRef = SomeOtherClassPtr(L, 2);
		--     const SomeOtherClass arg1 & = *(arg1_AsRef);
		--     int arg2 = luaL_checkint(L, 3);
		--     int rtn = instance->ComputeSomething(arg1, arg2);
		--     lua_pushinteger(L, rtn);
		--     return 1;
		-- ]]
		-- TODO: !! This doesn't support overloaded methods sharing a name!
		--  To do that, it'd need to check arguments and resolve the overload
		--  at runtime!
		--
		
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
		t[#t + 1] = "LUA_GLUE_TRY";
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
		-- Write code to grab each argument from the Lua stack and save it as a
		-- local variable.
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
		-- Write line which actually calls the target method in C code and
		-- saves the return value, if any.
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
			methodCall = methodCall .. node.FullName .. "::";
		end
		methodCall = methodCall .. methodName .. "(";
		-- Write the list of calls to the argument.
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
		-- If there's a return value, put it onto the Lua stack.
		-- Either way return the number of return values (0 or 1).
		t[#t + 1] = "\t" .. methodCall;
		if (returnType.Node.FullName == self.Creators.voidNode.FullName) then		
			t[#t + 1] = "\t" .. "return 0;";
		else			
			t[#t + 1] = "\t" .. returnTypeTm.put("rtn");
			t[#t + 1] = "\t" .. "return 1;";
		end	
		t[#t + 1] = "LUA_GLUE_CATCH";
		t[#t + 1] = "}";
		rtn.text = table.concat(t, "\n\t");
		return rtn;
	end,
	
	wrapGetterCall = function(self, methodOverloadNode) 
		-- Returns code which calls a C++ function, stores the return value,
		-- put that onto the Lua stack.
		-- So if it's "std::string SomeClass::GetName();" it's something like:
		--    std::string str = instance->GetName();
		--    lua_pushstring(L, str.c_str());
		
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
		-- Write line which actually calls the target method in C code and
		-- saves the return value, if any.
		local returnType = methodOverloadNode.Member.ReturnType;
		if (returnType.Node.FullName == self.Creators.voidNode.FullName) then
			-- Should be caught by earlier checks.
			error("Node is marked as a getter but returns nothing.");
		end
		local returnTypeTm = self:TypeManipulators(returnType);
		local methodCall = "";
		methodCall = methodCall .. tostring(returnType) .. " rtn = ";
		
		if (not methodOverloadNode.Member.Static) then
			methodCall = methodCall .. "instance->";
			--error("Node marked as getter (" .. methodOverloadNode.Node.FullName
			--	  .. " but is static.");
		else
			local mNode = self:getMetaNode(node);
			methodCall = methodCall .. node.Name .. "::";
		end
		methodCall = methodCall .. methodName .. "();";		
		t[#t + 1] = "\t" .. methodCall;
		t[#t + 1] = "\t" .. returnTypeTm.put("rtn");				
		return t;
	end,	
	
	wrapSetterCall = function(self, methodOverloadNode, nextStackIndex) 
		-- Returns code which calls a C++ function to set some value from the
		-- Lua stack.
		-- So if it's "void SomeClass::SetName(const std::string & value);" 
		-- it's something like:
		--    std::string value(lua_checkstring(L, 1));
		--    instance->SetName(value);
		
		check(self ~= nil, 'Argument "self" missing!');
		check(methodOverloadNode ~= nil, 'Argument "methodNode" missing!');
		check(methodOverloadNode.Member ~= nil, 'Argument "methodNode" has no Member defined!');		
		check(methodOverloadNode.Member.TypeName == "FunctionOverload", 'Argument "methodNode" must have Member defined as FunctionOverload.');
		check(nextStackIndex ~= nil, "nextStackIndex must be a number.");
		
		local node = methodOverloadNode.Node.Node;
		check(node ~= nil, "The methodNode had no parent?! How can that be?!");
		local rtn = {}
		local methodName = methodOverloadNode.Node.Name
		rtn.name = methodName;
		local t = { }
		-- Write line which actually calls the target method in C code and
		-- saves the return value, if any.
		local argument = self:findFirstArgument(methodOverloadNode);		
		if argument == nil then
			error("Node has no function arguments (" 
			      .. methodOverloadNode.Node.FullName .. ").");
		end
		local argumentType = argument.Member.Type;		
		local argumentTypeTm = self:TypeManipulators(argumentType);
		local methodCall = "";
		t[#t + 1] = "\t" .. argumentTypeTm.convertArgument("newValue", nextStackIndex);				
		if (methodOverloadNode.Member.Static) then
			error("Node marked as setter (" .. methodOverloadNode.Node.FullName
				  .. ") but is static.");
		end
		t[#t + 1] = "\tinstance->" .. methodName .. "(newValue);";
		return t;
	end,
	 
	checkArgumentIsNodeGeneratorType = function(self, name, args)
		-- Assertion helper function called by members of "NodeGenerator" to 
		-- make sure the tables sent in as "self" are properly formed.
		check2(args ~= nil, 'Argument "' .. name .. '" may not be nil.');
		check2(args.helperName ~= nil, 'Argument "' .. name .. '.helperName" may not be nil.');
		check2(args.originalNode ~= nil, 'Argument "' .. name .. '.originalNode" may not be nil.');
		check2(args.referenceType ~= nil, 'Argument "' .. name .. '.referenceType" may not be nil.');					
	end,
	
	createGlueHelperStruct = function(self, args)
		-- Given a table with a Node, its Lua glue reference type (usually a
		-- smart pointer of some kind) and a name of a helper struct, returns
		-- a C++ code string which defines a helper stuct with methods that call
		-- the class represented by the node Lua style.
		-- So if the class has "const std::string GetName()" that gets wrapped
		-- as "int GetName(lua_State * L)" which calls the same method.
		self:checkArgumentIsNodeGeneratorType("args", args);
				
		local t = { }				
		
		local funcs = self:findAllFunctionsInNode(args.originalNode);	
		t[#t + 1] = "struct " .. args.helperName .. "";
		t[#t + 1] = "{";
		t[#t + 1] = self:createHelperGc(args);
		t[#t + 1] = self:createHelperIndex(args);
		t[#t + 1] = self:createHelperNewIndex(args);
		t[#t + 1] = self:createHelperGlobalTableIndex(args);
		t[#t + 1] = self:createHelperGlobalTableNewIndex(args);
		for i = 1, #funcs do
			local node = funcs[i];
			local methodInfo = self:wrapMethod(node);
			t[#t + 1] = methodInfo.text;		
		end
		t[#t + 1] = "};";
		return table.concat(t, "\n\t");	
	end,

	createHelperGc = function(self, args)
		-- Returns the C++ code defining a Lua glue C function which is called
		-- when the Lua instance is garbage collected.  This code by default
		-- calls the destructor on the reference type.  So if its a smart 
		-- pointer it'll decrease the reference count.
		
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
	
	createHelperGlobalTableIndex = function(self, args)
		-- Lua glue C function called when the global table is index to return
		-- property like values.
		
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __global__index(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\t// Assume Lua will pass in table as first argument.";	
		t[#t + 1] = "\t\tstd::string index(luaL_checkstring(L, 2));";
		t[#t + 1] = "\t\treturn " .. metaNodeName .. "::GlobalIndex(L, index);"
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,
	
	createHelperGlobalTableNewIndex = function(self, args)
		-- Lua glue C function called when the global table is index to return
		-- property like values.
		
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __global__newindex(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\tlua_pushstring(L, \"Table is read-only.\");";
		t[#t + 1] = "\t\treturn lua_error(L);";
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,
	
	createHelperIndex = function(self, args)
		-- Creates a Lua glue C function which is called when the Lua instance
		-- is indexed.  This is basically a big if statement checking the 
		-- string name and returning the correct helper struct Lua glue C
		-- function or property.
		-- This actually passes to a public method "BlahLuaMetaData::Index".		
		--
		-- TODO: Move this into the ClassWrapper
		
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __index(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\tLUA_GLUE_TRY";	
		t[#t + 1] = "\t\t" .. args.referenceType.FullName .. " & ptr = " .. metaNodeName ..  "::GetInstance(L, 1);";
		t[#t + 1] = "\t\tstd::string index(luaL_checkstring(L, 2));";
		t[#t + 1] = "\t\treturn " .. metaNodeName .. "::Index(L, ptr, index);"
		t[#t + 1] = "\t\tLUA_GLUE_CATCH";
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,	
	
	createHelperNewIndex = function(self, args)
		-- Creates a Lua glue C function which is called when the Lua instance
		-- has a new value indexed.  This is what provides for property setter
		-- like functionality.
		
		self:checkArgumentIsNodeGeneratorType("args", args);
		
		local metaNodeName = self:getMetaNode(args.originalNode).FullName;
		local t = { }
		t[#t + 1] = "\tstatic int __newindex(lua_State * L)";
		t[#t + 1] = "\t{";	
		t[#t + 1] = "\t\t" .. args.referenceType.FullName .. " & ptr = " .. metaNodeName ..  "::GetInstance(L, 1);";
		t[#t + 1] = "\t\tstd::string index(luaL_checkstring(L, 2));";
		t[#t + 1] = "\t\tLUA_GLUE_TRY";
		t[#t + 1] = "\t\tif (" .. metaNodeName .. "::NewIndex(L, ptr, index, 3))"
		t[#t + 1] = "\t\t{";
		t[#t + 1] = "\t\t\treturn 0;";
		t[#t + 1] = "\t\t}";
		t[#t + 1] = "\t\tLUA_GLUE_CATCH";
		t[#t + 1] = "\t\t// TODO: Check inheritance chain and call the setters there if false returned."
		t[#t + 1] = "\t\tstd::stringstream ss;";
		t[#t + 1] = "\t\t" .. [[ss << "The property \"" << index << "\" cannot be set.";]];
		t[#t + 1] = "\t\treturn luaL_error(L, ss.str().c_str());;"		
		t[#t + 1] = "\t}";	
		return table.concat(t, "\n\t");	
	end,	
	
	getMetaNode = function(self, node)
		-- Given a node, returns its "meta node" containing the LuaGlue 
		-- wrapper stuff.
		-- Currently, for "Blah", this would be "BlahLuaMetaData".
		--
		local metaNodeName = node.FullName .. "LuaMetaData";
		local metaNode = self.RootNode:FindOrCreate(metaNodeName);
		return metaNode;
	end,
	
	ClassWrapper = { 
		-- like a nested class, this has the helper methods called by Lua
		-- It also defines a lot of constants referred to repeatedly in the
		-- glue, such as the name of the global table created.
		
		new = function(parent, node, referenceType)
			-- "parent" is some magical factory LuaGlue creator object in 
			--   charge of this one
			-- "node" is the thing we're actually wrapping
			-- "referenceType" is what that node uses as its reference type in Lua.			
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
			-- There's some stuff we have to add to the Glue which Macaroni
			-- can't really handle.  So we put it in the following C block.
			-- This includes stuff like the static data mapping metatable
			-- values to c functions, but also more complex things like the
			-- helper struct.  
			
			local blockHome = self.metaNode:FindOrCreate("functionPtrStructBlock");
			log:Write("Going to create block for function pointer struct.");
			local wrappedFunctions = self.parent:createGlueHelperStruct(self);
			local staticText = {};
			-- Writes the list of methods and strings which are used as the 
			-- meta table.
			local funcs = self.parent:findAllFunctionsInNode(self.originalNode, true);
			for i = 1, #funcs do
				local fNode = funcs[i]
				local tableEntry = '{"' .. fNode.Node.Name .. '", ' 
					.. self.helperName .. "::" .. fNode.Node.Name .. '}'; 
				staticText[#staticText + 1] = tableEntry;				
			end
			log:Write("Creating me a block.");
			local block = Block.Create(blockHome, "cpp",					
[[

#define LUA_GLUE_TRY try {
#define LUA_GLUE_CATCH } catch(const std::exception & ex){ return luaL_error(L, ex.what()); }

namespace
{	
	]] .. wrappedFunctions .. [[
	
	
	static const struct luaL_Reg globalTableMethods[]=
	{
		]] .. self:luaLRegConcat(staticText, ",\n\t")	.. [[ 
		{0, 0}
	};
	
	static const struct luaL_Reg globalTableMetaTableMethods[]=
	{
		{"__index", ]] .. self.helperName .. [[::__global__index},
		{"__newindex", ]] .. self.helperName .. [[::__global__newindex},
		{0, 0}
	};

	static const struct luaL_Reg metaTableMethods[]=
	{	
		//{"__eq", NodeLuaFunctions::__eq},
		{"__gc", ]] .. self.helperName .. [[::__luaGc},
		{"__index", ]] .. self.helperName .. [[::__index},
		{"__newindex", ]] .. self.helperName .. [[::__newindex},
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
				self.parent.RootNode:FindOrCreate("std::string"),
				self.parent.RootNode:FindOrCreate("std::stringstream"),
			});        
			for i = 1, #imports do
				log:Write("imports[".. i .. "]=".. tostring(imports[i]));
			end
			-- Mix all imports of the wrapped node with this one.
			local otherImports = self.originalNode.Member.ImportedNodes;
			if otherImports ~= nil then
				for i = 1, #otherImports do
					local otherImport = otherImports[i];
					imports[#imports + 1] = otherImport;
					success, args = pcall(function() 
						return self.parent:LuaWrapperArguments(otherImport);
					end)
					if success then
						imports[#imports + 1] = args.metaNode;
					end				
				end
			end
			
			local metaClass = Class.Create(CurrentLibrary, self.metaNode, imports, self.reason);    			    
			-- static bool IsType(lua_State * L, int index);        
		end,
		
		createIndexMethodBody = function(self, isInstance)			
			local first = true;				
			local t = {}			
			local funcs = self.parent:findAllFunctionsInNode(self.originalNode, false);
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
			local getters = self.parent:findAllGettersInNode(self.originalNode, not isInstance);
			for i = 1, #getters do
				local node = getters[i];
				local propName = node.Attributes[self.parent.LuaProperty]
				                                                 .ValueAsString;
				if (first) then					
					t[#t + 1] = 'if (index == "' .. propName .. '")';
					first = false;
				else
					t[#t + 1] = 'else if (index == "' .. propName ..'")';
				end
				t[#t + 1] = '{';
				local getCode = self.parent:wrapGetterCall(node);
				for j = 1, #getCode do
					t[#t + 1] = getCode[j];
				end				
				t[#t + 1] = '}';					
			end
			if not first then
				t[#t + 1] = 'else'
				t[#t + 1] = '{'
			end
			t[#t + 1] = "\tlua_pushnil(L);"
			if not first then
				t[#t + 1] = '}'	
			end
			t[#t + 1] = 'return 1;'	
			
			return table.concat(t, "\n\t");		
		end,
		
		createNewIndexMethodBody = function(self)			
			local first = true;				
			local t = {}						
			local setters = self.parent:findAllSettersInNode(self.originalNode);
			for i = 1, #setters do
				local node = setters[i];
				local propName = node.Attributes[self.parent.LuaProperty]
				                                                 .ValueAsString;
				if (first) then					
					t[#t + 1] = 'if (index == "' .. propName .. '")';
					first = false;
				else
					t[#t + 1] = 'else if (index == "' .. propName ..'")';
				end
				t[#t + 1] = '{';
				local getCode = self.parent:wrapSetterCall(node, 'nextStackIndex');
				for j = 1, #getCode do
					t[#t + 1] = getCode[j];
				end				
				t[#t + 1] = '\treturn true;';
				t[#t + 1] = '}';					
			end
			if not first then
				t[#t + 1] = 'else'
				t[#t + 1] = '{'
			end
			t[#t + 1] = "\treturn false;"
			if not first then
				t[#t + 1] = '}'		
			end
			
			return table.concat(t, "\n\t");		
		end,
		
		createEitherIndexMethod = function(self, methodName, isInstance)
			local node = self.metaNode:FindOrCreate(methodName);
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);		
			local rtnType = Type.New(self.parent.Creators.intNode, { });
			log:Write("Going to create Index " .. methodName);
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
									   rtnType,
									   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);
			if isInstance then
				local arg2 = fo1.Node:FindOrCreate("instance");
				local arg2Type = Type.New(self.referenceType, { Reference = true });
				Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);		
			end
			local arg3 = fo1.Node:FindOrCreate("index");			
			local arg3Type = Type.New(self.parent.Creators.stringNode, { Const = true, Reference = true });
			Variable.Create(arg3, "Access_Public", false, arg3Type, "", self.reason);		
							
			local methodBody = self:createIndexMethodBody(isInstance);						
			fo1:SetCodeBlock(methodBody, self.src);				
			
		end,
		
		createGlobalIndexMethod = function(self)
			self:createEitherIndexMethod("GlobalIndex", false);
		end,
		
		createIndexMethod = function(self)			
			self:createEitherIndexMethod("Index", true);
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
	// New global table is left at the top of the stack.
	
	// Now create a meta table for the global table.
	lua_newtable(L); // t
	luaL_register(L, 0, globalTableMetaTableMethods);
	lua_setmetatable(L, -2);	
	
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
		
		createNewIndexMethod = function(self)			
			local node = self.metaNode:FindOrCreate("NewIndex");
			-- NodePtr home, bool isInline, const Access access, const bool isStatic, 
			-- const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);		
			local rtnType = Type.New(self.parent.Creators.boolNode, { });
			log:Write("Going to create NewIndex.");
			local func = Function.Create(node, self.reason);
			local fo1 = FunctionOverload.Create(func, false, "Access_Public", true, 
									   rtnType,
									   false, self.reason);
			func = node.Member;		
			local arg1 = fo1.Node:FindOrCreate("L");		
			local arg1Type = Type.New(self.lua_StateNode, { Pointer = true });
			Variable.Create(arg1, "Access_Public", false, arg1Type, "", self.reason);
			local arg2 = fo1.Node:FindOrCreate("instance");
			local arg2Type = Type.New(self.referenceType, { Reference = true });
			Variable.Create(arg2, "Access_Public", false, arg2Type, "", self.reason);		
			local arg3 = fo1.Node:FindOrCreate("index");			
			local arg3Type = Type.New(self.parent.Creators.stringNode, { Const = true, Reference = true });
			Variable.Create(arg3, "Access_Public", false, arg3Type, "", self.reason);		
			local arg4 = fo1.Node:FindOrCreate("nextStackIndex");			
			local arg4Type = Type.New(self.parent.Creators.intNode, { Const = true });
			Variable.Create(arg4, "Access_Public", false, arg4Type, "", self.reason);		
							
			local methodBody = self:createNewIndexMethodBody();						
			fo1:SetCodeBlock(methodBody, self.src);				
			
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
			self:createNewIndexMethod();
			self:createIndexMethod();
			self:createGlobalIndexMethod();
		    
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
		log:Write("Wrapping " .. tostring(class) .. ".");
		local wrapArgs = generator:LuaWrapperArguments(class);
		log:Write("WTF:" .. tostring(wrapArgs));
		for k, v in pairs(wrapArgs) do
			log:Write("wrapArgs   " .. tostring(k) .. "=" .. tostring(v));
		end
		if wrapArgs.glueAlreadyExists == false then
			log:Write("Going to wrap.");
			generator:wrapClass(wrapArgs)
		else
			log:Write("Skipping wrap.");
		end
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

