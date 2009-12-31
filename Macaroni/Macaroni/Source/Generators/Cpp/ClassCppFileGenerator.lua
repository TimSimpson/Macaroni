require "Cpp/Common";
require "Cpp/ClassFileGenerator";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

ClassCppFileGenerator = {

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
        
        setmetatable(args, ClassCppFileGenerator);
        
        return args;
    end,    
    
    classBegin = function(self)
        
    end,
    
    classBody = function(self)
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
        self:write("/* Adopted Global Functions */\n"); 
        self:write("namespace {\n"); 
        self:globalPrototypes();
        self:iterateMembers(self.node.Member.GlobalNodes, "Access_Private");       
        self:write("} // end anonymous namespace\n");
        self:iterateMembers(self.node.Member.GlobalNodes, "Access_Public"); 
        self:write("/* End globals. */\n");
    end,   
    
    globalPrototypes = function(self)
        local globals = self.node.Member.GlobalNodes;        
        for i=1, #globals do
            local node = globals[i];
            if (node.Member ~= nil and
                node.Member.TypeName == TypeNames.Function and
                node.Member.Access == "Access_Private") then
                    self:writeFunctionDefinition(node);                
            end
        end
    end,
    
    includeGuardFooter = function(self)        
        self.writer:write("#endif // end of " .. self:getGuardName());    
    end,
    
    includeGuardHeader = function(self)
        local guardName = self:getGuardName();
        self.writer:write("#ifndef " .. guardName .. "\n");
        self.writer:write("#define " .. guardName .. "\n");       
    end,
    
    includeStatements = function(self)
        local class = self.node.Member;
        local imports = class.ImportedNodes;
        -- Put the include to this classes H file.
        local hFile = '#include "' .. self.node.Name .. '.h"\n';
        self.writer:write(hFile);
        for i = 1, #imports do                      
            local import = imports[i];        
            self:writeInclude(import);                
        end
    end,
    
    iterateClassMembers = function(self, nodeChildren)
        self:iterateMembers(nodeChildren);
        --[[for i=1, #nodeChildren do
            node = nodeChildren[i];
            self:parseMember(node);
        end]]--
    end,       
    
    -- Entry function.
    parse = function(self)      
        if (not self.isNested) then  
            self:includeGuardHeader();
            self.writer:write('\n');
            self:includeStatements();
            self.writer:write('\n');
            self:usingStatements();
            self.writer:write('\n');            
            self:namespaceBegin();
            self.writer:write('\n');
            self:globals();
            self.writer:write('\n');
        end
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
        self:write(self.node.Name .. "::" .. self.node.Name .. "(");
        self:writeArgumentList(node);
        self:write(")\n");        
        self:writeTabs();
        self:write("{\n");
        self:addTabs(1);
        
        self:writeTabs();
        self:write(node.Member.CodeBlock .. "\n");
        
        self:addTabs(-1);        
        self:writeTabs();
        self:write("}\n");
    end,
    
    ["parse" .. TypeNames.Function] = function(self, node)
        self:writeTabs();
        local func = node.Member;
        self:writeType(func.ReturnType);
        self:write(" ");
        if (not self:isNodeGlobal(node)) then
            self:write(self.node.Name .. "::" .. node.Name);
        else
            self:write(node.Name);
        end        
        self:write("(");
        self:writeArgumentList(node);
        self:write(")\n");        
        
        self:writeTabs();
        self:write("{\n");
        self:addTabs(1);
        
        self:writeTabs();
        self:write(node.Member.CodeBlock .. "\n");
        
        self:addTabs(-1);        
        self:writeTabs();
        self:write("}\n");
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
            ClassCppFileGenerator.New({isNested = true, node = node, writer = self.writer});
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
        if (self:isNodeGlobal(node)) then
            self:writeVariableDefinition(node);
        end        
    end,        
    
    writeInclude = function(self, import)        
        local statement = IncludeFiles.createStatementForNode(import);
        if (statement ~= nil) then self:write(statement); end
    end,      
    
    writeUsing = function(self, import) 
        if (import.Node.IsRoot) then
            -- Avoid writing using statements for Nodes belonging to ROOT.
            return;
        end
        local statement = nil;
        local generateWarning = true;
        if (import.Member ~= nil) then
            if (import.Member.TypeName == TypeNames.Class) then
                statement = "using " .. import:GetPrettyFullName("::") .. ";\n";
            elseif (import.Member.TypeName == TypeNames.Primitive) then
                generateWarning = false;
            end            
        end
        if (statement == nil) then
            if (generateWarning) then 
                statement = "/* ~ <(I don't know how to generate a using statement for " .. tostring(import) .. ".) */\n";
            else
                statement = "";
            end
        end
        self:write(statement);
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