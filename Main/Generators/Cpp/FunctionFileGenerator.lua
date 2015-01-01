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

    insertIntoNamespaces = false,
    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(args)
        assert(args.node ~= nil);
        assert(args.targetLibrary ~= nil);
        args.functionNode = args.node.Node
        assert(args.functionNode ~= nil);
        args.fo = args.node.Element
        assert(args.fo.TypeName == TypeNames.FunctionOverload)
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
            --local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter();
            --if (writer == nil) then
            --    error(tostring(errorNumber) .. " " .. errorMsg, 2);
            --end
            args.writer = args.path:CreateFile();--writer;
        end

        setmetatable(args, FunctionFileGenerator);
        args.libDecl = LibraryDecl(args.targetLibrary);
        return args;
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

    -- Entry function.
    WriteHeaderDefinition = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");

        if not (self.fo.Access.VisibleInLibrary or self.livesWithClass) then
            return  -- Skip node with no public visibility
        end
        if self.insertIntoNamespaces then
            self:namespaceBegin(self.functionNode.Node);
        end
        self:writeTabs();

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

    WriteCppDefinition = function(self)
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
        self:write("\n");

        self:writeFunctionCodeBlock(self.fo);
        if self.insertIntoNamespaces then
            self:namespaceEnd(self.node.Node.Node);
        end
    end,

};

Util.linkToSubClass(FileGenerator, FunctionFileGenerator);


return {
    FunctionFileGenerator=FunctionFileGenerator
}
