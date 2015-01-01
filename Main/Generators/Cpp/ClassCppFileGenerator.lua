--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------
require "Macaroni.Model.Cpp.Access";
require "Cpp/Common";
require "Cpp/ClassFileGenerator";
require "Cpp/ClassHFileGenerator";
local FunctionGenerator = require "Cpp/FunctionFileGenerator";
require "Cpp/NodeInfo"
require "Cpp/DependencySection";
require "Cpp/LibraryConfigGenerator";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local FunctionCppFileGenerator = FunctionGenerator.FunctionCppFileGenerator
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

ClassCppFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(self)
        assert(self.node ~= nil);
        log:Write(tostring(self.node) .. ' . .. . . . . . . .TIME TO ROCK!');
        if (self.path == nil) then
            assert(self.writer ~= nil);
        else
            --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
            --local writer, errorMsg, errorNumber = io.open(self.path.AbsolutePath, 'w+'); --self.path:NewFileWriter();
            --if (writer == nil) then
            --    error(tostring(errorNumber) .. " " .. errorMsg, 2);
            --end
            self.writer = self.path:CreateFile();--writer;
        end

        setmetatable(self, ClassCppFileGenerator);
        self.libDecl = LibraryDecl(self.targetLibrary);
        log:Write("Created new ClassCppFileGenerator");

        self.internalHeaderGenerator = ClassHFileGenerator.new{
            node = self.node, internalDef=true,
            targetLibrary=self.targetLibrary, writer = self.writer};
        return self;
    end,

    classBegin = function(self)

    end,

    classBody = function(self)
		log:Write("Class body for " .. tostring(self.node) .. ".")
        --self:classBegin();
        self:iterateClassMembers(self.node.Children);
    end,

    classEnd = function(self)

    end,

    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_CPP";
        return guardName;
    end,

    getNodeAlias = function(self, node)
        return node.Name;
    end,

    globals = function(self)
		local globalNodes = self.node.Member.GlobalNodes;
		local privateCount = 0;
		local hiddenCount = 0;
		local publicCount = 0;
		for i = 1, #globalNodes do
			local gNode = globalNodes[i]
			local access = nil;
			if gNode.Member ~= nil then
				access = gNode.Member.Access;
			end
			if access == Access.Hidden then hiddenCount = hiddenCount + 1; end
			if access == Access.Private then
				privateCount = privateCount + 1;
			end
			if access == Access.Public then publicCount = publicCount + 1; end
		end

		if privateCount > 0 then
			self:write([[/* Define "Private" adopted global functions and variables. */]]
			           .. "\n");
			self:globalPrototypes(Access.Private);
		end

        if hiddenCount > 0 then
			self:write([[/* "Hidden" adopted global functions and variables. */]]
					   .. "\n");
			self:write("// Defines and uses an anonymous namespace.\n");
			self:write("namespace {");
			self:addTabs(1);
			self:writeVerbose("/* Prototypes */\n");
			self:globalPrototypes(Access.Hidden);
			self:writeVerbose("/* Implementation */\n");
			self:iterateMembers(self.node.Member.GlobalNodes, Access.Hidden);
			self:addTabs(-1);
			self:write("}\n");
			self:write("// End of anonymous namespace.\n");
			self:write("\n");
		end

        if privateCount > 0 then
			self:write([[/* Implementation of "Private" adopted global functions and variables. */]]
					   .. "\n");
			self:iterateGlobalMembers(self.node.Member.GlobalNodes, Access.Private);
			self:write("\n");
		end

		if publicCount > 0 then
			self:write([[/* "Public" adopted global functions and variables. */]]
					   .. "\n");
			self:iterateGlobalMembers(self.node.Member.GlobalNodes, Access.Public);
			self:write("\n");
		end
    end,

    globalPrototypes = function(self, access)
        local globals = self.node.Member.GlobalNodes;
        for i=1, #globals do
            local node = globals[i];
            if (node.Member ~= nil) then
                if (node.Member.TypeName == TypeNames.Function) then
					for j=1, #node.Children do
						local nodeJ = node.Children[j]
						if (nodeJ.Member.Access == access) then
							self:writeGlobalPrototype(nodeJ);
						end
					end
				elseif (node.Member.TypeName == TypeNames.FunctionOverload) then
					if (node.Member.Access == access) then
						self:writeGlobalPrototype(node);
					end
				end
            end
        end
    end,

    includeStatements = function(self)
		self:writePrivateHeader();
        self:writeImplementationIncludeStatements();
    end,

    iterateClassMembers = function(self, nodeChildren)
        self:iterateMembers(nodeChildren);
        --[[for i=1, #nodeChildren do
            node = nodeChildren[i];
            self:parseMember(node);
        end]]--
    end,

    -- Entry function.
    -- TODO: Stop using this and delete it.
    parse = function(self)
        if (not self.isNested) then
            self:includeGuardHeader();
            self:classTopBlocks();
            self:write('\n');
			self:write("// The following configures symbols for export if needed.\n");
			self:write("#define " .. LibraryCreate(self.targetLibrary) .. "\n");
			self:write("\n");
            self:includeStatements();
            self:write('\n');
            self:parseCppIncludeBlocks();
            self:usingStatements();
            self:write('\n');
            self:globals();
            self:write('\n');
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end
        self:classBody();
        if (not self.isNested) then
            self:write('\n');
            self:namespaceEnd(self.node.Node);
            self:write('\n');
            self:classBottomBlocks();
            self:includeGuardFooter();
        end
    end,

    parseCppIncludeBlocks = function(self)
		for i=1, #self.node.Children do
			local node = self.node.Children[i]
			if node.TypeName == TypeNames.Block and
			   node.Member.Id == "cpp-include" then
			   self:writeBlockCodeBlock(node.Member);
			end
		end
    end,

    ["parse" .. TypeNames.Block] = function(self, node)
        local block = node.Member;
        if (block.Id == "cpp") then
			self:writeBlockCodeBlock(block);
        end
    end,

    ["parse" .. TypeNames.Constructor] = function(self, node)
    	check(node.Member ~= nil and node.Member.TypeName == TypeNames.Constructor,
    	      "Argument 'node' must be have a Member which is a Constructor.")
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseConstructorOverload(overloadNode);
		end
    end,

    ["parse" .. TypeNames.ConstructorOverload] = function(self, node)
    	check(node.Member ~= nil and node.Member.TypeName == TypeNames.ConstructorOverload,
    	      "Argument 'node' must be a Member which is a ConstructorOverload.")
        if MACARONI_VERSION ~= "0.2.3" then
            if (not node.Member.RequiresCppFile) then
                return;
            end
        else -- START LEGACY STUFF
            if (node.Member.Inline) then
                self:write("//~<(Skipping inline constructor.)\n");
                return;
            end
            if (not node.Member.HasCodeBlock) then
                self:write('//~<(Skipping constructor with no code block "' .. node.FullName .. '")\n');
                return;
            end
        end -- END LEGACY STUFF
        if node.Member.Access.VisibleInLibrary and self.libDecl then
			self:writeTabs();
			self:write(self.libDecl .. "\n");
		end
        self:writeTabs();
        if self.isNested then -- Qualify child class via parent name.
            self:write(self.parent.Name .. "::");
        end
        self:write(self.node.Name .. "::" .. self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")\n");
        if MACARONI_VERSION ~= "0.1.0.27" then
            if (node.Member.ExceptionSpecifier) then
                self:write(" " .. node.Member.ExceptionSpecifier);
            end
        else
            if (node.Member.ThrowSpecifier) then
                self:write(" throw()");
            end
        end
        self:writeTabs();
        --[[local assignments = node.Member.Assignments;
        local seenOne = false;
        for i = 1, #assignments do
            local assignment = assignments[i];
            if (not seenOne) then
                self:write(": ");
                seenOne = true;
            else
                self:write(", ");
            end
            self:write(assignment.Variable.Name .. "("
                       .. assignment.Expression .. ")");
        end
        if (seenOne) then
            self:write("\n");
            self:writeTabs();
        end]]--
        self:writeConstructorAssignments(node.Member.Assignments);

        self:writeFunctionCodeBlock(node.Member);
        --self:write("{\n");
        --self:addTabs(1);
        --
        --self:writeTabs();
        --self:write(node.Member.CodeBlock .. "\n");
        --
        --self:addTabs(-1);
        --self:writeTabs();
        --self:write("}\n");
    end,

    ["parse" .. TypeNames.Destructor] = function(self, node)
		check(#node.Children == 1, "Destructor must have one child.");
		local overload = node.Children[1];

        if MACARONI_VERSION ~= "0.2.3" then
            if (not overload.Member.RequiresCppFile) then
                return;
            end
        else -- START LEGACY STUFF
            if (overload.Member.Inline) then
                self:write("//~<(Skipping inline destructor.)\n");
                return;
            end
            if (not overload.Member.HasCodeBlock) then
                self:write('//~<(Skipping destructor with no code block "' .. node.FullName .. '")\n');
                return;
            end
        end -- END LEGACY STUFF

        if overload.Member.Access.VisibleInLibrary and self.libDecl then
			self:writeTabs();
			self:write(self.libDecl .. "\n");
		end
        self:writeTabs();
        if self.isNested then -- Qualify child class via parent name.
            self:write(self.parent.Name .. "::");
        end
        self:write(self.node.Name .. "::~" .. self.node.Name .. "(");
        self:writeArgumentList(overload);
        self:write(")\n");
        if MACARONI_VERSION ~= "0.1.0.27" then
            if (overload.Member.ExceptionSpecifier) then
                self:write(" " .. overload.Member.ExceptionSpecifier);
            end
        else
            if (overload.Member.ThrowSpecifier) then
                self:write(" throw()");
            end
        end
        self:writeTabs();
        self:write("{\n");
        self:addTabs(1);

        self:writeTabs();

        self:write(overload.Member.CodeBlock .. "\n");

        self:addTabs(-1);
        self:writeTabs();
        self:write("}\n");
    end,

    ["parse" .. TypeNames.Function] = function(self, node, insertIntoNamespaces)
		if insertIntoNamespaces then
			self:namespaceBegin(node.Node);
		end
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseFunctionOverload(overloadNode);
		end
		if insertIntoNamespaces then
			self:namespaceEnd(node.Node);
		end
    end,

	["parse" .. TypeNames.FunctionOverload] = function(self, node,
	                                                   insertIntoNamespaces)
        local args = {
            node = node,
            targetLibrary=self.targetLibrary,
            writer = self.writer,
            insertIntoNamespaces=insertIntoNamespaces,
            livesWithClass = true,
            isNested=isNested,
            ownedByClass=true,
        };

        -- Add the class prefix if necessary.
        if (not self:isFunctionOverloadNodeGlobal(node)) then
            args.classPrefix = ""
            if self.isNested then -- Qualify child class via parent name.
                args.classPrefix = self.parent.Name .. "::"
            end
            args.classPrefix = args.classPrefix .. self.node.Name .. "::";
        end

        local gen = FunctionFileGenerator.new(args);
        gen:addTabs(self.tabs);
        gen:WriteCppDefinition()
    end,

    parseMember = function(self, node, insertIntoNamespaces)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;
        if (typeName == TypeNames.Class) then
            -- Pass the new generator the same writer as this class.
            gen = ClassCppFileGenerator.new({
                isNested = true,
                node = node,
                parent = self.node,
                writer = self.writer,
                targetLibrary = self.targetLibrary});
            handlerFunc = function(self)
                self:writeAfterTabs("// Nested class " .. node.Name .. "\n");
                gen:parse();
            end;
        else
            handlerFunc = self["parse" .. typeName];
        end

        if (handlerFunc ~= nil) then
            handlerFunc(self, node, insertIntoNamespaces);
        else
            self:writeTabs();
            self:write("//     ~ Have no way to handle node " .. node.Name .. " with Member type " .. typeName .. ".\n");
        end
    end,

    ["parse" .. TypeNames.Namespace] = function(self)
        self:writeTabs();
        self:write("//TODO: Create parseNamespace functionality for Namespaces within classes.");
    end,

    ["parse" .. TypeNames.Variable] = function(self, node, insertIntoNamespaces)
		if insertIntoNamespaces then
			self:namespaceBegin(node.Node.Node);
		end
        if (self:isNodeGlobal(node)) then
            self:writeVariableDefinition(node, true);
        elseif (node.Member.Static) then
            self:writeTabs();
            local variable = node.Member;
            self:writeType(variable.Type, not insertIntoNamespaces);
            self:write(' ' .. node.Node.Name .. '::' .. node.Name);
            if (variable.Initializer ~= "") then
                self:write(' = ');
                self:write(variable.Initializer);
            end
            self:write(";\n");
        end
        if insertIntoNamespaces then
			self:namespaceEnd(node.Node.Node);
		end
    end,

    WriteBottomBlocks = function(self)
        self:classBottomBlocks();
    end,

    WriteForwardDeclarations = function(self)
        self.internalHeaderGenerator:WriteForwardDeclarations();
    end,

    writeGlobalPrototype = function(self, foNode)
		check(foNode.Member.Access.CppKeyword == "private",
			  "Why write a prototype for a global method that is not " ..
			  "private in the CPP file?");
		if foNode.Member.Access == Access.Private then
			self:namespaceBegin(foNode.Node.Node);
		end
		self:writeFunctionOverloadDefinition(foNode);
		self:write(";\n");
		if foNode.Member.Access == Access.Private then
			self:namespaceEnd(foNode.Node.Node);
		end
    end,

    WriteHeaderDefinitions = function(self)
        self.internalHeaderGenerator:WriteHeaderDefinitions();
    end,

    WriteIncludeStatements = function(self)
        self.internalHeaderGenerator:WriteIncludeStatements();
    end,

    WriteImplementation = function(self)
        if (not self.isNested) then
            self:globals();
            self:write('\n');
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end
        self:classBody();
        if (not self.isNested) then
            self:write('\n');
            self:namespaceEnd(self.node.Node);
            self:write('\n');
        end
    end,

    WriteImplementationIncludeStatements = function(self)
        self:writeImplementationIncludeStatements()
        self:write('\n');
        self:parseCppIncludeBlocks();
    end,

    writeImplementationIncludeStatements = function(self)
        log:Write(tostring(self.node) .. ' INCLUDE STATEMENTS! *_*');
        local class = self.node.Member;
        local imports = class.ImportedNodes;

        for i = 1, #imports do
            local import = imports[i];
            if (import.Member ~= nil) then
                self:writeInclude(import);
            else
                self:write('// Skipping hollow imported node "' .. import.FullName .. '"' .. "   \n");
            end
        end
    end,

    writeInclude = function(self, import)
        local statement = IncludeFiles.createStatementForNode(import);
        if (statement ~= nil) then self:write(statement); end
    end,

    WritePreDefinitionBlocks = function(self)
        self.internalHeaderGenerator:WritePreDefinitionBlocks();
    end,

    WritePostDefinitionBlocks = function(self)
        self.internalHeaderGenerator:WritePostDefinitionBlocks();
    end,

    writePrivateHeader = function(self)
		log:Write("Creating private header.");
		self:write("/*--------------------------------------------------------------------------*\n");
		self:write(" * Internal header.                                                         *\n");
		self:write(" *--------------------------------------------------------------------------*/\n");
        self.internalHeaderGenerator:parse();
		self:write("\n\n");
		self:write("/*--------------------------------------------------------------------------*\n");
		self:write(" * Definition.                                                              *\n");
		self:write(" *--------------------------------------------------------------------------*/\n");
    end,

    WriteTopBlocks = function(self)
        self:classTopBlocks();
    end,

    writeUsing = function(self, import)
        self:write(NodeInfoList[import].using);
    end,

    WriteUsingStatements = function(self)
        self:usingStatements();
        self:write('\n');
    end,

    usingStatements = function(self)
		local class = self.node.Member;
        local imports = class.ImportedNodes;
        for i = 1, #imports do
            local import = imports[i];
            self:writeUsing(import);
        end
    end,

};

Util.linkToSubClass(FileGenerator, ClassCppFileGenerator);
