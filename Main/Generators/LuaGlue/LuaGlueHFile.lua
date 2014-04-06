require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/NodeInfo";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

LuaGlueHFile = {
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

        setmetatable(args, LuaGlueHFile);
        return args;
    end,

    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_LUA_GLUE_H";
        return guardName;
    end,

    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");

        self:includeGuardHeader();
        self:write("#include <string>\n");
        self:write(NodeInfoList[self.node].useHeavyDef(self.targetLibrary) .. '\n');
        self:write(NodeInfoList[self.ptrNode].useHeavyDef(self.targetLibrary) .. '\n');
        self:write("\n");
        self:write("struct lua_State;\n");
        self:write("\n");
        self:namespaceBegin(self.node.Node);
            self:writeStructDefinition();
        self:namespaceEnd(self.node.Node);
        self:includeGuardFooter();
    end,

    writeStructDefinition = function(self)
        self:write([[
struct ]] .. self.node.Name .. [[LuaMetaData
{
    static bool IsType(lua_State * L, int index);
    static int OpenInLua(lua_State * L);
    static ]] .. self.ptrNode.Name .. [[ & GetInstance(lua_State * L, int index);
    static int Index(lua_State * L, ]] .. self.ptrNode.Name .. [[ & ptr, const std::string & index);
    static void PutInstanceOnStack(lua_State * L, const ]] .. self.ptrNode.Name .. [[ & ptr);
};
        ]]);
    end,
};

Util.linkToSubClass(FileGenerator, LuaGlueHFile);
