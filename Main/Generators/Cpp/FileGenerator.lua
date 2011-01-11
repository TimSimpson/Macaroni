require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

FileGenerator = {
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,
        
    getNodeAlias = function(self, node) -- The Alias of a Node for this kind of file.
        return node.FullName;
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
        --self:write(NodeHelper.namespaceBegin(namespaceNode));
        local fs = namespaceNode.FullName;
        local names = Node.SplitComplexName(fs);
        for i = 1, #names do
            self:write("namespace " .. names[i] .. " { ");
        end
        self:write("\n");
    end,    
    
    namespaceEnd = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        --self:write(NodeHelper.namespaceEnd(namespaceNode));
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
    
    
    
    write = function(self, text)
        if (type(text) ~= "string") then
            error("String was expected in call to write, but got " .. type(text) .. " instead.", 2);
        end
        --log:Write("DEBUG:" .. debug.traceback());       
        self.writer:Write(text);
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
  
    writeConstructorAssignments = function(self, assignments)        
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
        end        
    end,
    
    --[[ Writes a function definition, not including the Class name before the
         function name. ]]--
    writeFunctionDefinition = function(self, functionNode)
        self:writeTabs();
        local func = functionNode.Member;
        if (functionNode.Member.Static) then
            self:write("static ");
        end
        if (functionNode.Member.Inline) then
            self:write("inline ");
        end
        self:writeType(func.ReturnType);
        self:write(" " .. functionNode.Name .. "(");
        self:writeArgumentList(functionNode);
        self:write(")");
        if (func.Const) then
            self:write(" const");
        end         
    end,
        
    writeType = function(self, type)    
        check(self ~= nil, "Member method called without instance.");
        check(type ~= nil, 'Argument 2 "type" can not be null.');    
        local typeUtil = TypeUtil.new();
        local str = typeUtil:createTypeDefinition(type, self.attemptShortName);
        self:write(str);        
    end,
    
    writeVariableDefinition = function(self, node, includeInitializer, prefixWithClassName)
        self:writeTabs();
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