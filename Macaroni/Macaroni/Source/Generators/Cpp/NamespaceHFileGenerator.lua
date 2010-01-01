require "Cpp/Common";
require "Cpp/FileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

NamespaceHFileGenerator = {

    isNested = false,
    node = nil,
    tabs = 0,
    writer = nil,
    
    new = function(args)
        if (args == nil or args.node == nil or (args.path == nil and args.writer == nil)) then
            error('Expected a table to create new instance with fields "node" and "path" or "writer".', 2);
        end        
        if (args.path == nil) then
            assert(args.writer ~= nil);
        else
            local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
            if (writer == nil) then
                error(tostring(errorNumber) .. " " .. errorMsg, 2);                
            end
            args.writer = writer;
        end
        
        setmetatable(args, NamespaceHFileGenerator);   

        args.minors = args:createMinorNodeList();
                 
        return args;
    end,
    
    createMinorNodeList = function(self)
        local rtn = {}
        for i = 1, #(self.node.Children) do
            local n = self.node.Children[i];               
            if (self:shouldIncludeNode(n)) then
                rtn[#rtn + 1] = n;
            end
        end
        return rtn;
    end,
    
    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("_") .. "___H";
        return guardName;
    end,
    
    includeStatements = function(self)    
        local statements = IncludeFiles.getListOfStatementsForNodeList(self.minors);
        self.writer:write("/* ~ Includes ~ */\n");
        for i = 1, #statements do
            self.writer:write(statements[i]);
        end 
    end,  
        
    parse = function(self)
        check(self ~= nil, "Instance method called without self.");
        check(self.writer ~= nil, "Instance writer missing.");
        self:includeGuardHeader();
        self.writer:write('\n');
        self:includeStatements();            
        self.writer:write('\n');            
        self:namespaceBegin(self.node);
        self.writer:write('\n');
        
        self:iterateMembers(self.minors);
        --for i = 1, #self.minors do
        --    self:write("// " .. self.minors[i].FullName .. "\n");
        --end
        
        --self.writer:write("class " .. self.node.Name .. ";\n");
        --self.writer:write('\n');
        --self:classPublicGlobals();
        --self.writer:write('\n');
        --self:classBody();
        
        self.writer:write('\n');
        self:namespaceEnd(self.node);
        self.writer:write('\n');
        self:includeGuardFooter();       
    end,
        
    parseMember = function(self, node)
        local m = node.Member;
        if (m == nil) then
            self:writeTabs();
            self:write("// No member info- " .. node.Name .. "\n");
        end
        local typeName = m.TypeName;
        local handlerFunc = nil;        
        if (typeName == TypeNames.Typedef) then           
            handlerFunc = self.parseTypedef;
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
    
    shouldIncludeNode = function(self, node)
        return (node.Member ~= nil 
                and node.Member.TypeName ~= TypeNames.Class
                and node.AdoptedHome == nil);
    end,
    
};

Util.linkToSubClass(FileGenerator, NamespaceHFileGenerator);