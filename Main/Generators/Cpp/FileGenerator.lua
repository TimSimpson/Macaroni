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
require "Cpp/Common";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;


FileGenerator = {
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,

    classTopBlocks = function(self)
        for i = 1, #self.node.Children do
            local child = self.node.Children[i]
            if child.TypeName == TypeNames.Block then
                block = child.Member
                if block.Id == "top" then
                    self:writeBlockCodeBlock(block)
                end
            end
        end
    end,

    classBottomBlocks = function(self)
        for i = 1, #self.node.Children do
            local child = self.node.Children[i]
            if child.TypeName == TypeNames.Block then
                block = child.Member
                if block.Id == "bottom" then
                    self:writeBlockCodeBlock(block)
                end
            end
        end
    end,

    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_H";
        return guardName;
    end,

    getNodeAlias = function(self, node) -- The Alias of a Node for this kind of file.
        return node.FullName;
    end,

    includeConfigFile = function(self)
        WriteLibraryConfigFileInclude(self.targetLibrary, self.writer)
    end,

    includeGuardFooter = function(self)
        self:write("#endif // end of " .. self:getGuardName());
    end,

    includeGuardHeader = function(self)
        check(self ~= nil, "Member called as static.");
        check(self.writer ~= nil, "The 'writer' field was set to nil. :(");
        local guardName = self:getGuardName();
        self:write("#ifndef " .. guardName .. "\n");
        self:write("#define " .. guardName .. "\n");
    end,

    isNodeGlobal = function(self, node)
    	return (node.Node ~= self.node);
    end,

    isFunctionOverloadNodeGlobal = function(self, node)
    	return (node.Node.Node ~= self.node);
	end,

    -- Iterates members of the list. Use access to filter which ones.
    iterateMembers = function(self, nodeChildren, access, insertIntoNamespaces)
		if insertIntoNamespaces == nil then
			insertIntoNamespaces = false
		end
        for i=1, #nodeChildren do
            local node = nodeChildren[i];
            if (node.Member ~= nil) then
				local memberType = node.Member.TypeName
                if (access == nil or node.Member.Access == access) then
                    self:parseMember(node, insertIntoNamespaces);
                elseif (node.Member.Access == nil) then
					if (memberType == "Function" or memberType == "Constructor"
						or memberType == "Destructor") then
						self:iterateMembers(node.Children, access)
					else
						self:write("/* ~<( Nil access for Node :" .. tostring(node) .. ".) */\n");
					end
                end
            end
        end
    end,

    -- Unlike the other method, this will open up and break out of namespaces
    -- before each definition.
    iterateGlobalMembers = function(self, nodeChildren, access)
		self:iterateMembers(nodeChildren, access, true);
    end,

    namespaceBegin = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        if not namespaceNode.IsRoot then
			--self:write(NodeHelper.namespaceBegin(namespaceNode));
			local fs = namespaceNode.FullName;
			local names = Node.SplitComplexName(fs);
			for i = 1, #names do
				self:write("namespace " .. names[i] .. " { ");
			end
			self:write("\n");
		end
    end,

    namespaceEnd = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        if not namespaceNode.IsRoot then
			--self:write(NodeHelper.namespaceEnd(namespaceNode));
			local names = Node.SplitComplexName(namespaceNode.FullName);
			for i = 1, #names do
				self:write("} ");
			end
			self:write("// End " .. namespaceNode.FullName);
			self:write("\n");
		end
    end,

    parseMember = nil,

    --["parse" .. TypeNames.Typedef] = function(self, node)
        --self:writeTabs();
        --local typedef = node.Member;
        --self:write("typedef ");
        --self:writeType(typedef.Type);
        --self:write(node.Name .. ";\n");
    --end,

    ["parse" .. TypeNames.Variable] = function(self, node)
        self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n\n");
    end,

    write = function(self, text)
        if (type(text) ~= "string") then
            error("String was expected in call to write, but got " .. type(text) .. " instead.", 2);
        end
        --log:Write("DEBUG:" .. debug.traceback());
        self.writer:Write(text);
    end,

    writeVerbose = function(self, text)
		if CPP_GENERATE_VERBOSE then
			self:write(text)
		end
    end,

    writeAfterTabs = function(self, text)
        self:writeTabs();
        self:write(text);
    end,

    writeTabs = function(self)
        for i = 1, self.tabs do
            self:write('\t');
        end
    end,

    -- Given the Node of a Function, writes the argument list.
    writeArgumentList = function(self, node, writeDefaultArgs)
        writeDefaultArgs = writeDefaultArgs
        if (nil == writeDefaultArgs) then
            writeDefaultArgs = self.isHeader
        end
        local seenOne = false;
        local args = node.Element.Arguments
        for i = 1, #args do
            local c = args[i];
            if (c.Member.TypeName == TypeNames.Variable) then
                if (seenOne) then
                    self:write(", ");
                else
                    seenOne = true;
                end
                self:writeType(c.Member.Type);
                self:write(" ");
                self:write(c.Name);
                if (writeDefaultArgs and c.Member.Initializer ~= "") then
                    self:write(" = ");
                    self:write(c.Member.Initializer);
                end
            end
        end
    end,

    writeFileName = function(self, name)
		name = string.gsub(name, "\\", "/");
		self:write(name);
    end,

    writeBlockCodeBlock = function(self, member)
		check(self ~= nil, "Method called without instance.")
		local code = member.Code;
		local reason = member.ReasonCreated;
		local source = reason.Source;
        local redirect = true;
        --TODO: Put a "redirect" property on Block.
        if (string.sub(member.Node.Node.Name, -string.len("LuaMetaData"))
            == "LuaMetaData") then
            redirect = false;
        end
		self:writeCodeBlock(code, source, redirect);
    end,

    writeCodeBlock = function(self, code, source, redirect)
        self:writeTabs();
        if redirect then
			self:write("#line " .. source.Line .. ' "');
			self:writeFileName(source.FileName.Name);
			self:write('"' .. "\n");
		else
			self:write("// This code was generated by ");
			self:writeFileName(source.FileName.Name);
			self:write('\n');
		end

        -- Write the code as it was so the #line directive will match up.
        self:write(code .. "\n");
        self:write("#line " .. self.writer.LineNumber + 1 .. ' "');
        self:writeFileName(self.writer.FilePath)
        self:write('"' .. "\n");
    end,

    writeConstructorAssignments = function(self, assignments)
    	check(self ~= nil, "Method called without instance.")
    	check(assignments ~= nil, "Argument #2 (assignments) must be list.")
        local seenOne = false;
        mt = getmetatable(assignments)
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
        end
    end,

    writeFunctionHeaderBody = function(self, element, isCtor)
        -- The function body as it is seen (or not seen) in the header.
        if element.IsPureVirtual == true then
            self:write(" = 0;\n");
        elseif element.UsesDefault == true then
            self:write(" = default;\n");
        elseif element.IsDeleted == true then
            self:write(" = delete;\n");
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

    writeFunctionExceptionSpecifier = function(self, element)
        if MACARONI_VERSION ~= "0.1.0.27" then
            if (element.ExceptionSpecifier) then
                self:write(" " .. element.ExceptionSpecifier);
            end
        else
            if (element.ThrowSpecifier) then
                self:write(" throw()");
            end
        end
    end,

    writeFunctionCodeBlock = function(self, memberWithBlock)
		check(self ~= nil, "Method called without instance.");
		check(memberWithBlock.CodeBlock ~= nil, "Member " ..memberWithBlock.Node.FullName.. " does not have code block.");

		local code = memberWithBlock.CodeBlock;
		local source = memberWithBlock.CodeSource;
		local redirect = memberWithBlock.CodeBlockShouldAddRedirect;
		check(source ~= nil, "Member " ..memberWithBlock.Node.FullName.. " does not have attached source.");
		self:writeTabs();
		self:write("{\n");
        self:addTabs(1);

		self:writeCodeBlock(code, source, redirect);

		self:addTabs(-1);
        self:writeTabs();
        self:write("}\n");
    end,

    --[[ Writes a function definition, not including the Class name before the
         function name. ]]--
    writeFunctionDefinition = function(self, foNode, calledFromClassWriter)
    	check(self ~= nil, "Method called without instance.");
    	check(foNode.Member ~= nil, "functionNode must have instance");
    	check(foNode.Member.TypeName == "Function", "Node must be Function, not "
    		  .. tostring(foNode.Member.TypeName));
    	for i = 1, #foNode.Children do
    		self:writeFunctionOverloadDefinition(foNode.Children[i], calledFromClassWriter)
    	end

    end,

    --[[ Writes a function overload definition, not including the Class name
         before the function name. ]]--
    writeFunctionOverloadDefinition = function(self, foNode,
		calledFromClassWriter, calledForFriendDefinition, writeDefaultArgs)
		-- foNode - the FunctionOverload node
		-- calledFromClassWriter
		-- calledForFriendDefinition - If true, then write the entire
		--     fullName of the Node instead of just the name.
		calledForFriendDefinition = calledForFriendDefinition or false;
    	check(self ~= nil, "Method called without instance.");
    	check(foNode ~= nil, "foNode must be a Node, not nil.");
    	check(foNode.Member ~= nil, "functionNode must have instance");
    	check(foNode.Member.TypeName == "FunctionOverload", "Node must be FunctionOverload");
        -- self:writeTabs();
        if foNode.Member.Access.VisibleInLibrary and self.libDecl
            and not calledFromClassWriter then
			self:write(self.libDecl .. " ");
        end
        local func = foNode.Member;
        if (foNode.Member.Static) then
            self:write("static ");
        end
        if MACARONI_VERSION=="0.2.3" then
            if (foNode.Member.Inline) then
                self:write("inline ");
            end
        else
            if (foNode.Member.UsesInlineKeyword) then
                self:write("inline ");
            end
        end

        self:writeType(func.ReturnType);
        if not calledForFriendDefinition then
            self:write(" " .. foNode.Node.Name .. "(");
        else
            self:write(" (::" .. foNode.Node.FullName .. ") (");
        end
        self:writeArgumentList(foNode, writeDefaultArgs);
        self:write(")");
        if (func.Const) then
            self:write(" const");
        end
        if (func.UsesOverrideKeyword) then
            self:write(" override");
        end
        self:writeFunctionExceptionSpecifier(func)
    end,

    writeImplementationInclude = function(self, import)
        local statement = IncludeFiles.createStatementForNode(import);
        if (statement ~= nil) then self:write(statement); end
    end,

    writeUsing = function(self, import)
        self:write(NodeInfoList[import].using);
    end,

    writeTemplateParameterList = function(self, tplOwningElement)
        WriteTemplateParameterList(tplOwningElement,
                                   function(arg) self:write(arg) end);
        self:write("\n");
        self:writeTabs();
    end,

    writeType = function(self, type, attemptShortNameArg)
        check(self ~= nil, "Member method called without instance.");
        check(type ~= nil, 'Argument 2 "type" can not be null.');
        attemptShortNameArg = attemptShortNameArg or self.attemptShortName
        local typeUtil = TypeUtil.new();
        local str = typeUtil:createTypeDefinition(type, attemptShortNameArg);
        self:write(str);
    end,

    writeVariableDefinition = function(self, node, includeInitializer, prefixWithClassName)
        -- self:writeTabs();
        local variable = node.Member;
        if (variable.Static) then
            self:write("static ");
        end
        self:writeType(variable.Type);
        self:write(' ');
        if (prefixWithClassName) then
            self:write(node.Node.Name .. '::');
        end
        self:write(node.Name);
        if (includeInitializer and variable.Initializer ~= "") then
            self:write(' = ');
            self:write(variable.Initializer);
        end
        self:write(";\n");
    end
};
