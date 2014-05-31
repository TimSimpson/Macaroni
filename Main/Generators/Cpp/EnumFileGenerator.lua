require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


EnumFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(args)
        if (args == nil or args.node == nil or (args.path == nil and args.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            args.writer = args.path:CreateFile();
        end

        setmetatable(args, EnumFileGenerator);

        args.minors = args:createMinorNodeList();

        args.libDecl = LibraryDecl(args.targetLibrary);

        return args;
    end,

    createMinorNodeList = function(self)
        return { self.node }
    end,


    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "___H";
        return guardName;
    end,

    hasEntriesInLibrary = function(self, targetLibrary)
        return self.node.Element.OwnedBy(targetLibrary);
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

    memberEligibleForOutput = function(self, member)
        local typeName = member.TypeName;
        local handlerFunc = nil;
        if (typeName == TypeNames.Typedef) then
            handlerFunc = self.parseTypedef;
        else
            handlerFunc = self["parse" .. typeName];
        end
        return handlerFunc ~= nil;
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

        self:iterateMembers(self.minors);

        self:write('\n');
        self:namespaceEnd(self.node.Node);
        self:write('\n');
        self:includeGuardFooter();
    end,

    parseMember = function(self, node)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;
        if (typeName == TypeNames.Typedef) then
            handlerFunc = self.parseTypedef;
        else
            handlerFunc = self["parse" .. typeName];
        end

        if (handlerFunc ~= nil) then
            handlerFunc(self, node);
        else
            self:writeTabs();
            self:write("//     ~ Have no way to handle node " .. node.Name .. " with Member type " .. typeName .. ".\n");
        end
    end,

    ["parse" .. TypeNames.Enum] = function(self, node)
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

    shouldIncludeNode = function(self, node)
        if (node.Member ~= nil
                and node.Member.TypeName ~= TypeNames.Class
                and node.AdoptedHome == nil) then
            if (node.HFilePath == nil) then
                return true;
            end
            local attr = node.Annotations["Macaroni::Cpp::LightDef"];
            if (attr ~= nil) then
                return true;
            end
            local attr = node.Annotations["Macaroni::Cpp::UseLightDef"]
            if attr ~= nil and not attr.ValueAsBool then
                return true;
            end
        end
        return false;
    end,

};

Util.linkToSubClass(FileGenerator, EnumFileGenerator);

return {
    EnumFileGenerator=EnumFileGenerator
}
