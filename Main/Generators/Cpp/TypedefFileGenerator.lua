require "Cpp/Common";
require "Cpp/FileGenerator";
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


TypedefFileGenerator = {

    insideClass = false,
    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        assert(self ~= nil)
        assert(self.node ~= nil)
        assert(self.writer ~= nil)
        assert(self.node.TypeName == "Typedef");
        self.typedef = self.node.Element
        setmetatable(self, TypedefFileGenerator);

        self.minors = { self.node } ;

        self.libDecl = LibraryDecl(self.targetLibrary);

        return self;
    end,

    WriteForwardDeclarations = nil,

    WriteIncludeStatements = function(self)
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

    WriteForwardDeclarationsOfDependencies = nil,

    WritePreDefinitionBlocks = nil,

    WriteHeaderDefinitions = function(self)
        if not self.insideClass then
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end

        self:writeTabs();
        self:write("typedef ");
        self:writeType(self.typedef.Type);
        self:write(self.node.Name .. ";\n");

        if not self.insideClass then
            self:write('\n');
            self:namespaceEnd(self.node.Node);
        end
    end,

};

Util.linkToSubClass(FileGenerator, TypedefFileGenerator);
