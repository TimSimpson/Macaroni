require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

FileGenerator = {
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,
    
    -- given node A::B::C, returns array { A, A::B, A::B::C }
    createPathListFromNode = function(self, node)
        check(node ~= nil, "Cannot iterate nil node!");
        local i = 0;
        local itr = node;
        while (not itr.IsRoot) do
            i = i + 1;
            itr = itr.Node;
        end        
        local rtn = {}
        local itr2 = node;
        while (not itr2.IsRoot) do
            rtn[i] = itr2;
            i = i - 1;
            itr2 = itr2.Node;
        end
        return rtn;        
    end,   
    
    getNodeAlias = function(self, node) -- The Alias of a Node for this kind of file.
        return node.FullName;
    end,    
    
    includeGuardFooter = function(self)        
        self.writer:write("#endif // end of " .. self:getGuardName());    
    end,

    includeGuardHeader = function(self)
        check(self ~= nil, "Member called as static.");
        check(self.writer ~= nil, "The 'writer' field was set to nil. :(");
        local guardName = self:getGuardName();
        self.writer:write("#ifndef " .. guardName .. "\n");
        self.writer:write("#define " .. guardName .. "\n");       
    end,
    
      
    isNodeGlobal = function(self, node)
        return (node.Node ~= self.node);
    end,
    
    -- Iterates members of the list. Use access to filter which ones.
    iterateMembers = function(self, nodeChildren, access)
        for i=1, #nodeChildren do
            node = nodeChildren[i];
            if (node.Member ~= nil) then
                if (access == nil or node.Member.Access == access) then
                    self:parseMember(node);
                elseif (node.Member.Access == nil) then
                    self:write("/* ~<( Nil access for Node :" .. tostring(node) .. ".) */\n");
                end
            end
        end
    end,       
    
    namespaceBegin = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local fs = namespaceNode.FullName;
        local names = Node.SplitComplexName(fs);
        for i = 1, #names do
            self.writer:write("namespace " .. names[i] .. " { ");
        end
        self.writer:write("\n");
    end,    
    
    namespaceEnd = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local names = Node.SplitComplexName(namespaceNode.FullName);
        for i = 1, #names do
            self:write("} ");
        end
        self:write("// End namespace ");
        self:write("\n");
    end,
    
    parseMember = nil,  

    ["parse" .. TypeNames.Typedef] = function(self, node)
        self:writeTabs();
        local typedef = node.Member;
        self:write("typedef ");
        self:writeType(typedef.Type);
        self:write(node.Name .. ";\n");
    end,
    
    ["parse" .. TypeNames.Variable] = function(self, node)
        self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");
    end,  
    
     -- Searches the TypeArgumentList for a TypeArgument with the given node.
    searchTypeArgumentListForNode = function(self, typeArgList, node)
        check(typeArgList ~= nil, "TypeArgument list can't be nil.");
        check(node ~= nil, "Node can't be nil.");
        for i = 1, #typeArgList do
            local tArg = typeArgList[i];
            if (tArg.Node == node) then
                return tArg;
            end
        end
        return nil;
    end,   
    
    write = function(self, text)
        if (type(text) ~= "string") then
            error("String was expected in call to write, but got " .. type(text) .. " instead.", 2);
        end
        --print("DEBUG:" .. debug.traceback());       
        self.writer:write(text);
    end,    
    
    writeTabs = function(self)
        for i = 1, self.tabs do
            self.writer:write('\t');
        end
    end,
    
    -- Given the Node of a Function, writes the argument list.
    writeArgumentList = function(self, node)
        local seenOne = false;
        for i = 1, #(node.Children) do
            local c = node.Children[i];            
            if (c.Member.TypeName == TypeNames.Variable) then
                if (seenOne) then
                    self:write(", ");
                else
                    seenOne = true;
                end                
                self:writeType(c.Member.Type);
                self:write(" ");
                self:write(c.Name);               
            end            
        end
    end,
  
    --[[ Writes a function definition, not including the Class name before the
         function name. ]]--
    writeFunctionDefinition = function(self, functionNode)
        self:writeTabs();
        local func = functionNode.Member;
        self:writeType(func.ReturnType);
        self:write(" " .. functionNode.Name .. "(");
        self:writeArgumentList(functionNode);
        self:write(");\n");       
    end,
        
    writeType = function(self, type)    
        check(self ~= nil, "Member method called without instance.");
        check(type ~= nil, 'Argument 2 "type" can not be null.');    
        if (type == nil) then
            error("Type argument cannot be nil.", 2);           
        end
        if (type.Const) then
            self:write("const ");
        end
        local typeArguments = type.TypeArguments;
        if (typeArguments == nil or #typeArguments <= 0) then
            self:write(self:getNodeAlias(type.Node));
        else
            local nodeList = self:createPathListFromNode(type.Node);            
            for i = 1, #nodeList do
                local nodePart = nodeList[i];
                local typeArg = self:searchTypeArgumentListForNode(typeArguments, nodePart);
                self:write(nodePart.Name);
                if (typeArg ~= nil) then
                    self:write("<");                      
                    for i = 1, #typeArg.Arguments do                        
                        self:writeType(typeArg.Arguments[i]);
                    end
                    self:write(">");                     
                end
                if (i < #nodeList) then
                    self:write("::");
                end
            end            
        end        
        self:write(' ');
        if (type.Pointer) then
            self:write("* ");
        end
        if (type.Reference) then
            self:write("& ");
        end
        if (type.ConstPointer) then
            self:write("const ");
        end
    end,
    
    writeVariableDefinition = function(self, node)
        self:writeTabs();
        local variable = node.Member; 
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");
    end
};