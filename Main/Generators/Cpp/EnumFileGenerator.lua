require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


EnumHFileGenerator = {

    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        if (self == nil or self.node == nil or (self.path == nil and self.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end
        if (self.path == nil) then
            assert(self.writer ~= nil);
        else
            self.writer = self.path:CreateFile();
        end

        setmetatable(self, EnumHFileGenerator);

        self.minors = self:createMinorNodeList();

        self.libDecl = LibraryDecl(self.targetLibrary);

        return self;
    end,

    createMinorNodeList = function(self)
        return { self.node }
    end,


    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "___H";
        return guardName;
    end,

    includeStatements = function(self)
        local section = DependencySection.new();
        for i = 1, #self.minors do
            if CPP_GENERATE_VERBOSE then
                self:write("/* Adding minor " .. self.minors[i].FullName .. "*/\n");
            end
            section:add(self.minors[i]);
        end
        for i = 1, #section.list do
            local s = section.list[i];
            if (s.heavy == false) then
                self:writeVerbose("/* Light: " .. s.node.FullName .. "*/\n");
                self:write(NodeInfoList[s.node].useLightDef(self.targetLibrary));
                self:writeVerbose("/* END Light: " .. s.node.FullName .. "*/\n");
            else
                self:writeVerbose("/* Heavy: " .. s.node.FullName .. "*/\n");
                self:write(NodeInfoList[s.node].useHeavyDef(self.targetLibrary));
                self:writeVerbose("/* END Heavy: " .. s.node.FullName .. "*/\n");
            end
        end
    end,


    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        self:includeGuardHeader();
        --self:debugOutDependencyGraph();
        self:includeConfigFile();
        self:write('\n');
        self:includeStatements();
        self:write('\n');
        self:namespaceBegin(self.node.Node);
        self:write('\n');

        self:parseEnum(self.node);

        self:write('\n');
        self:namespaceEnd(self.node.Node);
        self:write('\n');
        self:includeGuardFooter();
    end,

    parseEnum = function(self, node)
        self:writeTabs();
        local enum = node.Member;
        self:writeAfterTabs("enum ");
        if enum.IsClassEnum then
            self:write("class ");
        end
        self:write(node.Name);
        if enum.SizeType then
            self:write(" ");
            self:writeType(enum.SizeType);
        end
        self:write("\n");
        self:writeAfterTabs("{\n");
        self:addTabs(1);
        local values = enum:GetValues()
        local one = false
        for k, v in pairs(values) do
            if one then
                self:write(",\n")
            else
                one=true
            end
            self:writeAfterTabs(tostring(k))
            if v then
                self:write("=" .. tostring(v))
            end
        end
        self:write("\n");
        self:addTabs(-1);
        self:writeAfterTabs("};\n");
    end,


};

Util.linkToSubClass(FileGenerator, EnumHFileGenerator);

EnumCppFileGenerator = {

    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        if (self == nil or self.node == nil or (self.path == nil and self.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end
        if (self.path == nil) then
            assert(self.writer ~= nil);
        else
            self.writer = self.path:CreateFile();
        end

        setmetatable(self, EnumCppFileGenerator);

        self.minors = self:createMinorNodeList();

        self.libDecl = LibraryDecl(self.targetLibrary);

        return self;
    end,

    createMinorNodeList = function(self)
        return { self.node }
    end,


    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "___H";
        return guardName;
    end,

    includeStatements = function(self)
        -- Write include statement. No inner header.
        self:write(NodeInfoList[self.node].useHeavyDef(self.targetLibrary));
    end,


    parse = function(self)
        return
        -- --TODO: Somehow add extra functions via annotations
        -- check(self ~= nil, "Instance method called without self.");
        -- check(self.writer ~= nil, "Instance writer missing.");
        -- self:includeGuardHeader();
        -- --self:debugOutDependencyGraph();
        -- self:includeConfigFile();
        -- self:write('\n');
        -- self:includeStatements();
        -- self:write('\n');
        -- self:namespaceBegin(self.node.Node);
        -- self:write('\n');

        -- self:parseEnum(self.node);

        -- self:write('\n');
        -- self:namespaceEnd(self.node.Node);
        -- self:write('\n');
        -- self:includeGuardFooter();
    end,

    -- hasEnumAttr = function(self, node, name)
    --     local attr = node.Annotations[name]
    --     if attr ~= nil then
    --         if not attr.IsString then
    --             error("The node " .. node.FullName .. " annotation value "
    --                   .. name .. " must be a string.")
    --         end
    --         return attr.ValueAsString
    --     end
    --     return nil
    -- end,

    parseEnum = function(self, node)
        --TODO: Add extra function bodies if possible
        -- local toStr = self:hasEnumAttr("Macaroni::Cpp::EnumToStringFunction")
        -- if toStr then

        -- end
    end,


};

Util.linkToSubClass(FileGenerator, EnumCppFileGenerator);

return {
    EnumHFileGenerator=EnumHFileGenerator,
    EnumCppFileGenerator=EnumCppFileGenerator
}
