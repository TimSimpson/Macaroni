require "Cpp/Common";
require "Cpp/ClassFileGenerator";

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
            local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
            if (writer == nil) then
                error(tostring(errorNumber) .. " " .. errorMsg, 2);                
            end
            args.writer = writer;
        end
        
        setmetatable(args, ClassHFileGenerator);                
        return args;
    end,
    
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
    end,
    
    classBegin = function(self)
        self.writer:write("class " .. self.node.Name .. "\n");
        self.writer:write("{\n");
        self:addTabs(1);
    end,
    
    classBody = function(self)
        self:classBegin();
        self:write("public:\n");
        self:iterateMembers(self.node.Children, "Access_Public");
        self:write("protected:\n");
        self:iterateMembers(self.node.Children, "Access_Protected");
        self:write("private:\n");
        self:iterateMembers(self.node.Children, "Access_Private");
        self:classEnd();
    end,
    
    classEnd = function(self)        
        self.writer:write("}; // End of class " .. self.node.Name .. "\n");
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
    
    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "_H";
        return guardName;
    end,
    
    getNodeAlias = function(self, node)
        return node.FullName;
    end,
    
    includeGuardFooter = function(self)        
        self.writer:write("#endif // end of " .. self:getGuardName());    
    end,
    
    includeGuardHeader = function(self)
        check(self ~= nil, "Member called as static.");
        check(self.writer ~= nil, "The 'writer' field was set to nil. :(");
        local guardName = self:getGuardName();
        self.writer:write("#ifndef " .. guardName .. "/* MAO! */\n");
        self.writer:write("#define " .. guardName .. "\n");       
    end,
    
    includeStatements = function(self)    
        local statements = IncludeFiles.getHFileIncludeStatementsForNode(self.node);
        self.writer:write("/* ~ Includes ~ */\n");
        for i = 1, #statements do
            self.writer:write(statements[i]);
        end                   
        -- alas, this is impossible.
        -- The NodePtrList type needs LuaGlue so the property can be accessed from the Class member.
        -- UPDATE: It does?
    end,  
    
    -- Entry function.
    parse = function(self)        
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        if (not self.isNested) then  
            self:includeGuardHeader();
            self.writer:write('\n');
            self:includeStatements();            
            self.writer:write('\n');            
            self:namespaceBegin();
            self.writer:write('\n');
        end
        self.writer:write("class " .. self.node.Name .. ";\n");
        self.writer:write('\n');
        self:classPublicGlobals();
        self.writer:write('\n');
        self:classBody();
        if (not self.isNested) then
            self.writer:write('\n');
            self:namespaceEnd();
            self.writer:write('\n');
            self:includeGuardFooter();
        end
    end,    
    
    ["parse" .. TypeNames.Constructor] = function(self, node)
        self:writeTabs();
        self:write(self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(");\n");        
    end,
    
    ["parse" .. TypeNames.Function] = function(self, node)
        self:writeFunctionDefinition(node);
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
        self:writeVariableDefinition(node);
        --[[self:writeTabs();
        local variable = node.Member;
        self:writeType(variable.Type);
        self:write(node.Name .. ";\n");]]--
    end,
    
};

Util.linkToSubClass(FileGenerator, ClassHFileGenerator);             