require "Cpp/Common";
require "Cpp/DependencySection";
require "Cpp/FileGenerator";
require "Cpp/NodeInfo";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;
 
LuaGlueCppFile = {
    node = nil,
    new = function(args)
        assert(args.node ~= nil);
        assert(args.ptrNode ~= nil);
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            --local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
            --if (writer == nil) then
            --    error(tostring(errorNumber) .. " " .. errorMsg, 2);                
            --end
            args.writer = args.path:CreateFile();--writer;
        end
        
        args.functionWrapperName = args.node.Name .. "LuaFunctions";
        args.globalTableName = args.node:GetPrettyFullName(".");
        args.metaTableName = args.node:GetPrettyFullName(".");
        args.attemptShortName = false;
        
        setmetatable(args, LuaGlueCppFile);                
        return args;        
    end,
    
    attemptShortName = false,
    
    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_LUA_GLUE_CPP";
        return guardName;
    end,
   
    getLuaGlueTypeName = function(self)
        return self.node.Name .. "LuaMetaData"
    end,
    
    getLuaGlueType = function(self, typeNode)
        local name = typeNode.FullName .. "LuaMetaData";
        local refNode = RootNode:Find(name);
        return refNode;
    end,
    
    getLuaReferenceType = function(self, typeNode)
        -- In the future, use attributes to get the props like this:
        --    typeNode.Attributes.LuaReferenceType -- points to a Node
        -- until then, hack! :)
        local name = nil;
        if (typeNode.FullName == "Macaroni::Model::Cpp::Access") then
            name = typeNode.FullName;      
        else
            name = typeNode.FullName .. "Ptr";
        end
        local refNode = RootNode:Find(name);
        return refNode;
    end,
    
    getLuaReferencedType = function(self, typePtrNode)     
        -- In the future, use attributes to get the props like this:
        --    typeNode.Attributes.LuaType -- points to a Node
        -- until then, hack! :)
        local name = nil;
        if (typePtrNode.FullName == "Macaroni::Model::Cpp::Access") then
            name = typeNode.FullName;
        else
            local original = typePtrNode.FullName;
            name = string.sub(original, (string.len(original) - 3) + 1);        
        end
        local refNode = RootNode:Find(name);
        return refNode;           
    end,
    
    includeStatements = function(self)  
        self:write(NodeInfoList[self.ptrNode].heavyDef);          
        self:write(NodeInfoList[self.node].heavyDef);
        
        local section = DependencySection.new();
        section:add(self.node);
        section:eraseDuplicates();
        for i = 1, #section.list do
            local s = section.list[i];
            local depInfo = NodeInfoList[s.node];
            self:write(depInfo.heavyDef);   
            if (depInfo.luaDef ~= nil) then
                self:write(depInfo.luaDef);
            end
        end       
        
        --[[local statements = IncludeFiles.getHFileIncludeStatementsForNode(self.node);
        self.writer:write("/* ~ Includes ~ */\n");
        for i = 1, #statements do
            self.writer:write(statements[i]);
        end ]]--                  
    end,  
    
    isFunctionNodeEligibleForWrapping = function(self, fNode)
        local member = fNode.Member;
        return (member ~= nil and
                member.TypeName == TypeNames.Function and
                member.Access == Access.Public and
                member.Static == false);
    end,
    
    iterateEligibleFunctions = function(self, action)       
        for i=1, #self.node.Children do
            local child = self.node.Children[i];
            if (self:isFunctionNodeEligibleForWrapping(child)) then
                action(child);
            end
        end   
    end,
    
    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        
        self:includeGuardHeader();
        self:writeIncludes();        
        self:namespaceBegin(self.node.Node);        
            self:write('\n');
            self:writeAnonNamespace();
            self:write('\n');
            self:writeFunctionWrapper();
            self:write('\n');
            self:writeLuaLRegs();    
            self:write('\n');       
            self:writeIndexFunction();
            self:write('\n');
            self:writePublicFunctions();
            self:write('\n');
        self:namespaceEnd(self.node.Node);
        self:includeGuardFooter();        
    end,
    
    writeAnonNamespace = function(self)
        self:write([[
namespace { 

	static inline void createUserData(lua_State * L, const ]] .. self.ptrNode.Name .. [[ & source)
	{ 
		void * memory = lua_newuserdata(L, sizeof(]] .. self.ptrNode.Name .. [[));
		]] .. self.ptrNode.Name .. [[ * instance = new (memory) ]] .. self.ptrNode.Name .. [[();		
		(*instance).operator=(source);
	}

	static inline ]] .. self.ptrNode.Name .. [[ & getInstance(lua_State * L, int index)
	{
		]] .. self.ptrNode.Name .. [[ * ptrToPtr = (]] .. self.ptrNode.Name .. [[ *) luaL_checkudata(L, index, "]] .. self.metaTableName .. [[");
		]] .. self.ptrNode.Name .. [[ & ptr = dynamic_cast<]] .. self.ptrNode.Name .. [[ &>(*ptrToPtr);
		return ptr;
	}        
	
	static inline ]] .. self.ptrNode.Name .. [[ & getInstance(lua_State * L)
	{
		return getInstance(L, 1);
	}

	static inline void putInstanceOnStack(lua_State * L, const ]] .. self.ptrNode.Name .. [[ & source)
	{
		if (!source) 
		{
			lua_pushnil(L);
		}
		else
		{
			createUserData(L, source);
			luaL_getmetatable(L, "]] .. self.metaTableName .. [[");
			lua_setmetatable(L, -2); 
		}
	}

} // End Anon namespace
]]);    
    end,
    
    writeFunctionWrapper = function(self)
        self:writeFunctionWrapperStart();
        self:iterateEligibleFunctions(function(fNode)
            self:writeFunctionWrapperForFunction(fNode.Member);       
        end);        
        self:writeFunctionWrapperEnd();
    end,
    
    writeFunctionWrapperStart = function(self)    
        self:write([[
 struct ]] .. self.functionWrapperName  .. [[
 {
 
    static int luaGc(lua_State * L)
    {
        ]] .. self.ptrNode.Name .. [[ * ptr = (]] .. self.ptrNode.Name .. [[ *) luaL_checkudata(L, 1, "]] .. self.metaTableName .. [[");
		ptr->~]] .. self.ptrNode.Name .. [[();
		return 0;
	}	

	static int __eq(lua_State * L)
	{
		]] .. self.ptrNode.Name .. [[ & a = getInstance(L, 1); 
		]] .. self.ptrNode.Name .. [[ & b = getInstance(L, 2);
		lua_pushboolean(L, a.get()==b.get() ? 1 : 0);
		return 1;
	}

	static int __index(lua_State * L)
	{
		]] .. self.ptrNode.Name .. [[ & ptr = getInstance(L);
		std::string index(luaL_checkstring(L, 2));
		return ]] .. self.node.Name .. [[LuaMetaData::Index(L, ptr, index);
	}
	]]);
	end,
	
	writeFunctionWrapperEnd = function(self)
	    self:write([[
}; // end of function wrappers
 ]]);
    end,
    
    writeFunctionWrapperForFunction = function(self, func)
        check(func ~= nil, self:isFunctionNodeEligibleForWrapping(func.Node), "#2 arg must be an eligible Function!");                
        self:write("\n\tstatic int " .. func.Node.Name .. "(lua_State * L)\n");
        self:write("\t{\n");
        self:write("\t\t" .. self.ptrNode.Name .. " & ptr = getInstance(L);\n");
        -- TODO: If there were some kind of annotation on each function, this next part would be much easier.
        -- Write out each argument and save reference in C++
        self:write("\t\t// TODO: Write out arguments here!\n");
        
        local luaGlueType = self:getLuaGlueType(func.ReturnType.Node);
        local rtnType = self:getLuaReferenceType(func.ReturnType.Node);
        local useWrappedLuaFunc = (luaGlueType ~= nil);
        if (rtnType == nil) then
            rtnType = func.ReturnType;
        end
        -- TODO: Whenever attributes are finished, the ptr type needs to be an actual
        -- Type, not the Node!
        self:write(rtnType.FullName);
        --self:writeType(rtnType);
        self:write(" __rtnValue = ptr->" .. func.Node.Name .. "(/* TODO: capabiltiy of arguments */);\n");
        if (useWrappedLuaFunc) then
            self:write(luaGlueType.Name .. "::PutInstanceOnStack(L, __rtnValue);\n");
        elseif (rtnType.FullName == "std::string") then
            self:write("lua_pushlstring(L, __rtnValue.c_str(), __rtnValue.length());\n");
        elseif (rtnType.Name == "int") then
            self:write("lua_pushinteger(L, __rtnValue);\n");
        else 
            self:write("// Do not know how to handle such a return type!\n");
        end
        
        self:write("\t\treturn 1;\n");
        self:write("\t}\n");
    end,
    
    writeIncludes = function(self)
        self:writeLuaInclude();
        self:write('#include "' .. self.node.Name .. 'Lua.h"\n');
        self:includeStatements();        
    end,
    
    writeIndexFunction = function(self)
        local glueName = self:getLuaGlueTypeName();
        local ptrName = self:getLuaReferenceType(self.node).Name;
        self:write("int " .. glueName .. "::Index(lua_State * L, " ..
            ptrName .. " & ptr, const std::string & index)\n");
        self:write("{\n");
        -- todo: allow properties
        local wroteOnce = false;
        self:iterateEligibleFunctions(function(fNode)
            self:write('\t');
            if (wroteOne) then 
                self:write("else ");
            end
            self:write('if (index == "' .. fNode.Name .. '")\n');
            self:write('\t{\n');
            self:write('\t\tlua_pushcfunction(L, ' .. self.functionWrapperName .. '::' .. fNode.Name .. ');\n');
            self:write('\t\treturn 1;\n');            
            self:write('\t}\n');
        end);
        self:write('\tlua_pushnil(L);\n');
        self:write('\treturn 1;\n');
        self:write("}\n");
    end,
    
    writeLuaInclude = function(self)
        self:write([[
 //extern "C" {
	#include <Lua/lua.h>
	#include <Lua/lauxlib.h>
	#include <Lua/lualib.h>
//}
]]);    
    end,
    
    writeLuaLRegs = function(self)
        self:write("static const struct luaL_Reg tableMethods[] =\n");
        self:write("{\n");
        self:iterateEligibleFunctions(function(fNode)
            if (fNode.Member.ReturnType.Static) then
                self:write('\t{"' .. fNode.Name .. '", ' .. 
                                self.functionWrapperName .. '::' ..
                                fNode.Name .. '},\n');     
            end
        end);
        self:write("\t{NULL, NULL}\n");
        self:write("};\n");
        
        self:write("static const struct luaL_Reg metaTableMethods[] =\n");
        self:write("{\n");
        self:iterateEligibleFunctions(function(fNode)
            if (not fNode.Member.ReturnType.Static) then
                self:write('\t{"' .. fNode.Name .. '", ' ..
                                self.functionWrapperName .. 
                                '::' .. fNode.Name .. '},\n');    
            end
        end);        
        self:write("\t{NULL, NULL}\n");
        self:write("};\n");
    end,    
    
    writePublicFunctions = function(self)
        local glueName = self:getLuaGlueTypeName();
        local ptrName = self:getLuaReferenceType(self.node).Name;
        self:write([[
]] .. ptrName .. [[ & ]] .. glueName .. [[::GetInstance(lua_State * L, int index)
{
	return getInstance(L, index);
}        

bool ]] .. glueName .. [[::IsType(lua_State * L, int index)
{
    // Copied this from the luaL_checkudata function
	void * p = lua_touserdata(L, index);
	bool returnValue = false;
	if (p != NULL) // is user data
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
}

int ]] .. glueName .. [[::OpenInLua(lua_State * L)
{	
	luaL_getmetatable(L, "]] .. self.metaTableName .. [[");
	if (lua_isnil(L, -1) != 1)
	{
		return 0; // Already loaded, DO NOT WASTE TIME DUMMY.
	}

	luaL_newmetatable(L, "]] ..self.metaTableName .. [["); // create metaTable
	luaL_register(L, NULL, metaTableMethods);

	// Creates or reuses a table called "]] .. self.globalTableName .. [[" 
    // and puts it in global scope.
	luaL_register(L, "]] .. self.globalTableName .. [[", tableMethods);
	return 1;
}
        ]]);
    end,
};

Util.linkToSubClass(FileGenerator, LuaGlueCppFile); 