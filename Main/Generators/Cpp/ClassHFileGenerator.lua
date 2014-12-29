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

ClassHFileGenerator = {

    isHeader = true,
    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,

    new = function(args)
        assert(args.node ~= nil);
        assert(args.targetLibrary ~= nil);
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

        setmetatable(args, ClassHFileGenerator);
        args.libDecl = LibraryDecl(args.targetLibrary);
        return args;
    end,


    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,

    classBegin = function(self)
		self:writeAfterTabs("// Define class " .. self.node.Name .. "\n");
        if self.node.Member.IsStruct then
            self:writeAfterTabs("struct ")
        else
            self:writeAfterTabs("class ")
        end
        if self.libDecl then
			self:write(self.libDecl .. " ");
        end
        self:write(self.node.Name .. "\n");
        parents = self.node.Member.Parents
		if #parents > 0 then
			self:classParents(parents)
		end
	    self:writeAfterTabs("{\n");
        self:addTabs(1);
    end,

    classParents = function(self, parents)
		self:write(": ");
		for i=1, #parents do
			parent = parents[i]
			if i > 1 then
				self:write(",\n  ");
			end
			access = parent.Access
			self:write(access.CppKeyword .. " ");
			if parent.IsVirtual == true then
				self:write("virtual ");
			end
			self:writeType(parent.Parent);
		end
		self:write("\n");
    end,

    classBody = function(self)
        self:classBegin();
        self:classFriends();
        --[[
        self:write("public:\n");
        self:iterateMembers(self.node.Children, "Access_Public");
        self:write("protected:\n");
        self:iterateMembers(self.node.Children, "Access_Protected");
        self:write("private:\n");
        self:iterateMembers(self.node.Children, "Access_Private");
        ]]--
        self:iterateMembers(self.node.Children);
        self:classEnd();
    end,

    classEnd = function(self)
		self:write('\n');
		self:addTabs(-1);
        self:writeAfterTabs("}; // End of class " .. self.node.Name .. "\n");
    end,

    classIncludeStatementBlocks = function(self)
		for i = 1, #self.node.Children do
			local child = self.node.Children[i]
			if child.TypeName == TypeNames.Block then
				block = child.Member
				if block.Id == "h-predef" or
                   (self.internalDef and block.Id == "h-internal-predef") then
					self:writeBlockCodeBlock(block)
					--self:write("// h-predef block: \n");
					--self:write(block.Code);
					--self:write("\n");
				end
			end
		end
    end,

    classPostDefBlocks = function(self)
        for i = 1, #self.node.Children do
            local child = self.node.Children[i]
            if child.TypeName == TypeNames.Block then
                block = child.Member
                if block.Id == "h-postdef" then
                    self:writeBlockCodeBlock(block)
                end
            end
        end
    end,

    classPublicGlobals = function(self)
        assert(self.node.Member ~= nil
               and
               self.node.Member.TypeName == TypeNames.Class);
        local globals = self.node.Member.GlobalNodes;
        if CPP_GENERATE_VERBOSE then
			self:writeAfterTabs("/* Public Global Members */\n");
		end
		self:iterateGlobalMembers(globals, Access.Public);
    end,

    classFriends = function(self)
        assert(self.node.Member ~= nil
               and
               self.node.Member.TypeName == TypeNames.Class);
        local friends = self.node.Member.FriendNodes;
        for i=1, #friends do
            friend = friends[i];
            if (friend.Member ~= nil
                and friend.Member.TypeName == TypeNames.FunctionOverload) then
                self:writeAfterTabs("friend ");
                self:writeFunctionOverloadDefinition(friend, false, true);
                self:write(";\n");
            elseif (friend.Member ~= nil
				     and friend.Member.TypeName == TypeNames.Function) then
				self:write([[
/* Putting friend on a free standing node which resolves to a function means
Macaroni will apply the 'friend' status to all function overloads.  To only
apply it to specific overloads, use the "~friend" keyword on the definitions
of those functions.  If this isn't possible, resort to a ~block. :( */]] .. '\n');
				for i = 1, #friend.Children do
					self:writeAfterTabs("friend ");
					self:writeFunctionOverloadDefinition(friend.Children[i],
						false, true);
    				self:write(";\n");
    			end
			elseif (friend.Member ~= nil
			         and friend.Member.TypeName == TypeNames.Class) then
			    self:writeAfterTabs("friend class ::" .. friend.FullName .. ";\n");
			else
                error("While adding friend declaration to " ..
                      tostring(self.node.FullName) .. ": friend specified was "
                      .. " a hollow Node (" .. friend.FullName .. ").");
                --self:writeAfterTabs("friend /*~<(What is this?!)*/ ::"
                --                    .. friend.FullName .. ";\n");
            end
        end
    end,

    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_H";
        return guardName;
    end,

    getNodeAlias = function(self, node)
        return node.FullName;
    end,

    includeStatements = function(self)
		local section = DependencySection.new();
        section:add(self.node);
        local globals = self.node.Member.GlobalNodes;
        for i = 1, #globals do
			section:add(globals[i]);
        end
        section:eraseNode(self.node);
        section:eraseDuplicates();
        for i = 1, #section.list do
            local s = section.list[i];
            if (s.heavy == false) then
				self:write(NodeInfoList[s.node].useLightDef(self.targetLibrary));
            else
				self:write(NodeInfoList[s.node].useHeavyDef(self.targetLibrary));
            end
        end

        --[[local statements = IncludeFiles.getHFileIncludeStatementsForNode(self.node);
        self.writer:write("/* ~ Includes ~ */\n");
        for i = 1, #statements do
            self.writer:write(statements[i]);
        end ]]--
    end,

    -- Entry function.
    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        if (not self.isNested) then
            self:includeGuardHeader();
            self:write('\n');
            if not self.internalDef then
                self:classTopBlocks();
            end
        end

        local reason = self.node.Member.ReasonCreated;
        local src = reason.Source;

        self:writeAfterTabs("// This class was originally defined in " .. tostring(src.FileName) .. "\n");

        if not self.isNested then
           self:includeConfigFile();
        end

        self:writeAfterTabs("// Forward declaration necessary for anything which also depends on this.\n");

        if (not self.isNested) then
            self:namespaceBegin(self.node.Node);
        end
        self:writeAfterTabs("class " .. self.node.Name .. ";\n");
        if (not self.isNested) then
            self:namespaceEnd(self.node.Node);
        end
        self:writeAfterTabs("\n");

        if (not self.isNested) then
            self:includeStatements();
            self:write('\n');
            self:write('\n');
			self:classIncludeStatementBlocks();
		end

		self:write('\n');
        self:classPublicGlobals();
        self:write('\n');

		if (not self.isNested) then
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end

        self:classBody();

        if (not self.isNested) then
            self:write('\n');
            self:classPostDefBlocks();
            self:namespaceEnd(self.node.Node);
            self:write('\n');
            if not self.internalDef then
                self:classBottomBlocks();
            end
            self:includeGuardFooter();
        end
    end,

    ["parse" .. TypeNames.Block] = function(self, node)
        local block = node.Member;
        if (block.Id == "h"
            or (self.internalDef and block.Id == "h-internal")) then
			self:writeBlockCodeBlock(block);
        end
    end,

    ["parse" .. TypeNames.Constructor] = function(self, node)
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseConstructorOverload(overloadNode);
		end
    end,

    ["parse" .. TypeNames.ConstructorOverload] = function(self, node)
        self:writeTabs();
        self:writeAccess(node.Member.Access);
        if (node.Member.Inline) then
            self:write("inline ");
        end
        if node.Member.Explicit then
            self:write("explicit ")
        end
        self:write(self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")");
        self:writeFunctionBody(node.Member, true);
    end,

    ["parse" .. TypeNames.Destructor] = function(self, node)
		check(#node.Children == 1, "Destructor must have one child.");
		local overload = node.Children[1];
        self:writeTabs();
        self:writeAccess(overload.Member.Access);
        if (overload.Member.Virtual) then
			self:write("virtual ");
		end
        if (overload.Member.Inline) then
            self:write("inline ");
        end
        self:write('~' .. self.node.Name .. "(");
        self:writeArgumentList(overload);
        self:write(")");
        self:writeFunctionBody(overload.Member, false);
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

    ["parse".. TypeNames.FunctionOverload] = function(self, node,
                                                      insertIntoNamespaces)
		if insertIntoNamespaces then
			self:namespaceBegin(node.Node.Node);
		end
		local ownedByClass = (node.Node.Node == self.node)
    	if (ownedByClass) then
    		if not self.internalDef then
    			if (not node.Member.Access.VisibleInHeader) then
    				return
    			end
    		end
    		self:writeTabs();
            self:writeAccess(node.Member.Access);
            if (node.Member.Virtual) then
    			self:write("virtual ");
    		end
        else
			self:writeTabs();
        end
        self:writeFunctionOverloadDefinition(node, ownedByClass);

        self:writeFunctionBody(node.Member, false);
		if insertIntoNamespaces then
			self:namespaceEnd(node.Node.Node);
		end
	end,

    parseMember = function(self, node, insertIntoNamespaces)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;
        -- Write nested class, but only if the HFilePath is not nil.
        if (typeName == TypeNames.Class and node.HFilePath == nil) then
			-- Pass the new generator the same writer as this class.
            gen = ClassHFileGenerator.new({isNested = true, node = node,
                                           targetLibrary=self.targetLibrary,
                                           writer = self.writer});
            gen:addTabs(self.tabs);
            handlerFunc = function(self)
				self:writeAfterTabs("public: \n");
				gen:parse()
            end;
        else
            handlerFunc = self["parse" .. typeName];
        end

        if (handlerFunc ~= nil) then
            handlerFunc(self, node, insertIntoNamespaces);
        else
            self:writeAfterTabs("//     ~ Have no way to handle node " .. node.Name .. " with Member type " .. typeName .. ".\n");
        end
    end,

    ["parse" .. TypeNames.Namespace] = function(self)
        self:writeTabs();
        self:write("//TODO: Create parseNamespace functionality for Namespaces within classes.");
    end,

    ["parse" .. TypeNames.Variable] = function(self, node, insertIntoNamespaces)
		if insertIntoNamespaces then
			self:namespaceBegin(node.Node);
		end
		self:writeTabs();
        if (node.Node == self.node) then
            self:writeAccess(node.Member.Access);
        end
        self:writeVariableDefinition(node);
        if insertIntoNamespaces then
			self:namespaceEnd(node.Node);
		end
        --[[self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");]]--
    end,

    writeAccess = function(self, access)
		local text = access.CppKeyword
		while #text < 10 do
			text = text .. ' ';
		end
		text = text .. ': ';
		self:write(text);
    end,

    writeFunctionBody = function(self, element, isCtor)
        if MACARONI_VERSION ~= "0.1.0.27" then
            if (element.ExceptionSpecifier) then
                self:write(" " .. element.ExceptionSpecifier);
            end
        else
            if (element.ThrowSpecifier) then
                self:write(" throw()");
            end
        end
        if element.IsPureVirtual == true then
            self:write(" = 0;\n");
        elseif element.UsesDefault == true then
            self:write(" = default;\n");
        elseif (not element.Inline) then
            self:write(";\n");
        else
            if (isCtor) then
                self:writeConstructorAssignments(element.Assignments);
            end
            self:write("\n");
            self:writeTabs();
            self:writeFunctionCodeBlock(element);
        end
    end,
};

Util.linkToSubClass(FileGenerator, ClassHFileGenerator);
