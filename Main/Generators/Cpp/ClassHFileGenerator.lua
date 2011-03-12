require "Cpp/Common";
require "Cpp/ClassFileGenerator";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";


local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

ClassHFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,
    
    new = function(args)          
        assert(args.node ~= nil);
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
        return args;
    end,
    
    
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,
    
    classBegin = function(self)
        self:write("class " .. self.node.Name .. "\n");
        self:write("{\n");
        self:addTabs(1);
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
        self:write("}; // End of class " .. self.node.Name .. "\n");
        self:addTabs(-1);
    end,
    
    classPublicGlobals = function(self)  
        assert(self.node.Member ~= nil 
               and 
               self.node.Member.TypeName == TypeNames.Class);
        local globals = self.node.Member.GlobalNodes;    
        self:write("/* Public Global Methods */\n");  
        self:iterateMembers(globals, "Access_Public");       
    end,
    
    classFriends = function(self)
        assert(self.node.Member ~= nil 
               and 
               self.node.Member.TypeName == TypeNames.Class);
        local friends = self.node.Member.FriendNodes;
        for i=1, #friends do
            friend = friends[i];
            if (friend.Member ~= nil and friend.Member.TypeName == TypeNames.Function) then
                self:write("friend ");
                self:writeFunctionDefinition(friend);
                self:write(";\n");
            else
                self:write("friend " .. friend.FullName .. ";\n");
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
        section:eraseDuplicates();
        for i = 1, #section.list do
            local s = section.list[i];
            if (s.heavy == false) then
                self:write(NodeInfoList[s.node].lightDef);
            else
                self:write(NodeInfoList[s.node].heavyDef);
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
        end
        
        local reason = self.node.Member.ReasonCreated;
        local src = reason.Source;
                
        self:write("// This class was originally defined in " .. tostring(src.FileName) .. "\n");
        self:write("// Forward declaration necessary if this depends on anything which also depend on this.\n");
        if (not self.isNested) then 
            self:namespaceBegin(self.node.Node);
        end
        self:write("class " .. self.node.Name .. ";\n");
        if (not self.isNested) then
            self:namespaceEnd(self.node.Node);
        end
        self:write("\n");
        
        if (not self.isNested) then  
            self:includeStatements();            
            self:write('\n');            
            self:namespaceBegin(self.node.Node);
            self:write('\n');
        end
        
        self:write('\n');
        self:classPublicGlobals();
        self:write('\n');
        self:classBody();
        if (not self.isNested) then
            self:write('\n');
            self:namespaceEnd(self.node.Node);
            self:write('\n');
            self:includeGuardFooter();
        end
    end,    
    
    ["parse" .. TypeNames.Block] = function(self, node)    
        local block = node.Member;            
        if (block.Id == "h") then
            self:write(block.Code);
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
        self:write(self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")");        
        if (not node.Member.Inline) then
            self:write(";\n");
        else
            self:writeConstructorAssignments(node.Member.Assignments);
            self:write("\n");
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(node.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end       
    end,
    
    ["parse" .. TypeNames.Destructor] = function(self, node)
        self:writeTabs();
        self:writeAccess(node.Member.Access);
        if (node.Member.Inline) then
            self:write("inline ");
        end
        self:write('~' .. self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")");
        if (not node.Member.Inline) then
            self:write(";\n");
        else
            self:write("\n");
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(node.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end        
    end,
    
    ["parse" .. TypeNames.Function] = function(self, node)
    	for i = 1, #(node.Children) do
    		local overloadNode = node.Children[i];
    		self:parseFunctionOverload(overloadNode);
		end
    end,    
    
    ["parse".. TypeNames.FunctionOverload] = function(self, node)
    	if (node.Node.Node == self.node) then
            self:writeAccess(node.Member.Access);
        end        
        self:writeFunctionDefinition(node);
        if (not node.Member.Inline) then
            self:write(";\n");
        else
            self:writeTabs();
            self:write("{\n");
            self:addTabs(1);
            
            self:writeTabs();
            self:write(node.Member.CodeBlock .. "\n");
            
            self:addTabs(-1);        
            self:writeTabs();
            self:write("}\n");
        end	
	end,
    
    parseMember = function(self, node)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;        
        if (typeName == TypeNames.Class) then
            -- Pass the new generator the same writer as this class.
            ClassHFileGenerator.New({isNested = true, node = node, writer = self.writer});
            handlerFunc = self.parseClass;
        else
            handlerFunc = self["parse" .. typeName];
        end
        
        if (handlerFunc ~= nil) then                       
            handlerFunc(self, node);
        else
            self:writeTabs();
            self:write("//     ~ Have no way to handle node " .. node.Name .. " with Member type " .. typeName .. ".\n");
        end 
    end,
    
    ["parse" .. TypeNames.Namespace] = function(self)
        self:writeTabs();
        self:write("//TODO: Create parseNamespace functionality for Namespaces within classes.");
    end,
    
    ["parse" .. TypeNames.Variable] = function(self, node)
        if (node.Node == self.node) then
            self:writeAccess(node.Member.Access);
        end
        self:writeVariableDefinition(node);
        --[[self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");]]--
    end,
    
    writeAccess = function(self, access)
        local text = nil;
        if (access == "Access_Public") then
            text = "public: ";
        elseif (access == "Access_Protected") then
            text = "protected: ";
        elseif (access == "Access_Private") then
            text = "private: ";
        else
            text = "/* ~ <(nil access?) */";
        end
        self:write(text);            
    end,
};

Util.linkToSubClass(FileGenerator, ClassHFileGenerator);             