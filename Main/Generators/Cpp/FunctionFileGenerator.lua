require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.Block";
require "Cpp/Common";
require "Cpp/ClassFileGenerator";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Cpp.ClassParent";
require "Macaroni.Model.Cpp.ClassParentList";
require "Macaroni.Model.Source";


local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


FunctionFileGenerator = {

    accessPrefix = "",
    classPrefix = "",
    insertIntoNamespaces = false,
    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        assert(self.node ~= nil);
        assert(self.targetLibrary ~= nil);
        self.functionNode = self.node.Node
        assert(self.functionNode ~= nil);
        self.fo = self.node.Element
        assert(self.fo.TypeName == TypeNames.FunctionOverload)
        assert(self.writer ~= nil)
        assert(self.fileType ~= nil)
        setmetatable(self, FunctionFileGenerator);
        self.libDecl = LibraryDecl(self.targetLibrary);
        return self;
    end,

    WriteForwardDeclarations = nil,  -- Only types do this.

    WriteImplementationIncludeStatements = function(self)
        local imports = self.fo.ImportedNodes;
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

    WriteIncludeStatements = function(self)
        --TODO: Make this do anything useful. CRUFT!!!
        local section = DependencySection.new();
        section:add(self.node);
        --- WTF? section:eraseNode(self.node);
        section:eraseDuplicates();
        for i = 1, #section.list do
            local s = section.list[i];
            print(s)
            if (s.heavy == false) then
                self:write(NodeInfoList[s.node].useLightDef(self.targetLibrary));
            else
                self:write(NodeInfoList[s.node].useHeavyDef(self.targetLibrary));
            end
        end
    end,

    WriteForwardDeclarationsOfDependencies = function(self)
        -- TODO: THIS!
    end,

    WriteHeaderDefinitions = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");

        if not (self.fo.Access.VisibleInLibrary or self.livesWithClass) then
            return  -- Skip node with no public visibility
        end
        if self.insertIntoNamespaces then
            self:namespaceBegin(self.functionNode.Node);
        end
        self:writeTabs();

        if (self.accessPrefix) then
            self:write(self.accessPrefix)
            self:write(' ')
        end

        -- Talk a trip to template town.
        self:writeTemplateParameterList(self.fo);

        -- classPrefix can be something like "public: virtual"
        if (self.classPrefix) then
            self:write(self.classPrefix)
            self:write(' ')
        end
        self:writeFunctionOverloadDefinition(self.node, self.ownedByClass,
            false, true);
        self:writeFunctionHeaderBody(self.fo, false);
        if self.insertIntoNamespaces then
            self:namespaceEnd(self.functionNode.Node);
        end
    end,

    WriteImplementation = function(self)
        check(self ~= nil, "Instance method called without self.");

        if MACARONI_VERSION ~= "0.2.3" then
            if (not self.fo.RequiresCppFile) then
                return;
            end
        else -- START LEGACY STUFF
            if (self.fo.Inline or self.fo.IsPureVirtual) then
                self:write("//~<(Skipping inline constructor.)\n");
                return;
            end
            if (not self.fo.HasCodeBlock) then
                self:write('//~<(Skipping constructor with no code block "' .. self.node.FullName .. '")\n');
                return;
            end
        end -- END LEGACY STUFF

        if self.insertIntoNamespaces then
            self:namespaceBegin(self.functionNode.Node);
        end
        if self.fo.Access.VisibleInLibrary and self.libDecl then
            self:writeTabs();
            self:write(self.libDecl .. "\n");
        end
        self:writeTabs();
        self:writeType(self.fo.ReturnType, not self.insertIntoNamespaces);

        self:write(" ");

        if self.classPrefix then
            self:write(self.classPrefix)
        end
        self:write(self.functionNode.Name);

        self:write("(");
        self:writeArgumentList(self.node, false);
        self:write(")");

        if (self.fo.Const) then
            self:write(" const");
        end

        if MACARONI_VERSION ~= "0.1.0.27" then
            if (self.fo.ExceptionSpecifier) then
                self:write(" " .. self.fo.ExceptionSpecifier);
            end
        else
            if (self.fo.ThrowSpecifier) then
                self:write(" throw()");
            end
        end
        local trt = self.fo.TrailingReturnType;
        if trt then
            self:write(" -> ");
            self:write(trt);
        end
        self:write("\n");

        self:writeFunctionCodeBlock(self.fo);
        if self.insertIntoNamespaces then
            self:namespaceEnd(self.node.Node.Node);
        end
    end,

    WritePreDefinitionBlocks = nil,  -- Functions don't own blocks.

    WritePostDefinitionBlocks = nil,  -- Functions don't own blocks.

    WriteUsingStatements = function(self)
        local imports = self.fo.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            self:writeUsing(import);
        end
    end,

};

Util.linkToSubClass(FileGenerator, FunctionFileGenerator);


return {
    FunctionFileGenerator=FunctionFileGenerator
}
