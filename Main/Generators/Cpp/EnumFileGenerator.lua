require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


EnumFileGenerator = {
    insideClass = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        assert(self ~= nil);
        assert(self.node ~= nil);
        assert(self.writer ~= nil);
        assert(self.targetLibrary ~= nil);
        self.enum = self.node.Element;
        assert(self.enum.TypeName == TypeNames.Enum);

        setmetatable(self, EnumFileGenerator);

        --self.minors = self:createMinorNodeList();
        self.libDecl = LibraryDecl(self.targetLibrary);
        return self;
    end,

    createMinorNodeList = function(self)
        return { self.node }
    end,

    writeEnumTopDecl = function(self)
        self:writeTabs();
        self:writeAfterTabs("enum ");
        if self.enum.IsClassEnum then
            self:write("class ");
        end
        self:write(self.node.Name);

        if self.enum.SizeType then
            self:write(" : ");
            self:writeType(self.enum.SizeType);
        end
    end,

    WriteForwardDeclarations = function(self)
        self:namespaceBegin(self.node.Node);
        self:write('\n');

        self:writeEnumTopDecl();
        self:write(";\n");

        self:namespaceEnd(self.node.Node);
        self:write('\n');
    end,

    WriteIncludeStatements = function(self)
        -- TODO: Need to include all types needed for the given type.
        -- if self.enum.SizeType then
        --     self:write(NodeInfoList[self.enum.SizeType].useHeavyDef(
        --         self.targetLibrary));
        -- end
    end,

    WriteImplementationIncludeStatements = function(self)
        local imports = self.enum.ImportedNodes;
        if imports then
            for i = 1, #imports do
                local import = imports[i];
                if (import.Member ~= nil) then
                    self:writeImplementationInclude(import);
                else
                    self:write('// Skipping hollow imported node "' .. import.FullName .. '"' .. "   \n");
                end
            end
        end
    end,

    WriteForwardDeclarationsOfDependencies = function(self)
        -- TODO: THIS!
    end,

    WriteHeaderDefinitions = function(self)
        if not self.insideClass then
            self:namespaceBegin(self.node.Node);
            self:write('\n');
            self:parseEnum(self.node);
            self:write('\n');
            self:namespaceEnd(self.node.Node);
        else
            self:parseEnum(self.node);
        end
    end,


    parseEnum = function(self, node)
        self:writeTabs();
        self:writeEnumTopDecl()
        self:write("\n");
        self:writeAfterTabs("{\n");
        self:addTabs(1);
        local values = self.enum:GetValues()
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

Util.linkToSubClass(FileGenerator, EnumFileGenerator);

return {
    EnumFileGenerator=EnumFileGenerator,
}
