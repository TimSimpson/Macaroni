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
    
    namespaceBegin = function(self)
        local fs = self.node.FullName;
        local names = Node.SplitComplexName(fs);
        for i = 1, #names - 1 do
            self.writer:write("namespace " .. names[i] .. " { ");
        end
        self.writer:write("\n");
    end,    
    
    namespaceEnd = function(self)
        local names = Node.SplitComplexName(self.node.FullName);
        for i = 1, #names - 1 do
            self:write("} ");
        end
        self:write("// End namespace ");
        self:write("\n");
    end,
    
    parseMember = nil,  

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
        if (type == nil) then
            error("Type argument cannot be nil.", 2);           
        end
        if (type.Const) then
            self:write("const ");
        end
        self:write(self:getNodeAlias(type.Node));
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