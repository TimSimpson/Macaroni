-- Eventually put this next part in C++:
package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Log";

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

function Generate(context, path)
    log.Init("Cpp");
    
    iterateNodes(context.Root.Children, path); 
end

function iterateNodes(nodeChildren, path)
    for i = 1, #(nodeChildren) do
        n = nodeChildren[i];
        parseNode(n, path);
    end
end

function parseClass(node, path)
    assert(node.Member.TypeName == TypeNames.Class);    
    local filePath = path:NewPath(".cpp");
    local cg = ClassGenerator.new{node = node, path = filePath};
    cg:parse();
end

function parseNamespace(node, path)
    assert(node.Member.TypeName == TypeNames.Namespace);    
    path:CreateDirectory();
    iterateNodes(node.Children, path);
end

function parseNode(node, path)
    if (not NodeHelper.worthIteration(node)) then
        return;
    end
    
    local m = node.Member;
    if (m == nil) then
        return;
    end
    local typeName = m.TypeName;
    local newPath = path:NewPath("/" .. node.Name);
    --if (newPath.IsDirectory) then
    --    newPath.CreateDirectory();
    --end
    
    local handlerFunc = nil;
    if (typeName == TypeNames.Namespace) then
        handlerFunc = parseNamespace;
    elseif (typeName == TypeNames.Class) then
        handlerFunc = parseClass;
    end
    
    if (handlerFunc ~= nil) then
        handlerFunc(node, newPath);
    else
        -- print("     ~ Skipping");
    end 
end


ClassGenerator = {

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
        
        setmetatable(args, ClassGenerator);
        ClassGenerator.__index = ClassGenerator;
        return args;
    end,
    
    addTabs = function(self, tabCount)
        self.tabs = self.tabs + tabCount;
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
        for i=1, #nodeChildren do
            node = nodeChildren[i];
            self:parseMember(node);
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
        for i = 1, #names -1 do
            self:write("} ");
        end
        self:write("// End namespace ");
        self:write("\n");
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
        self:writeVariableInfo(func.ReturnType);
        self:write(func.ReturnType.Node.Name);
        self:write(" ");
        self:write(self.node.Name .. "::" .. node.Name);
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
            ClassGenerator.New({isNested = true, node = node, writer = self.writer});
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
        
    end,
    
    write = function(self, text)
        if (type(text) ~= "string") then
            error("String was expected in call to write, but got " .. type(text) .. " instead.", 2);
        end
        --print("DEBUG:" .. debug.traceback());       
        self.writer:write(text);
    end,
    
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
                self:writeVariableInfo(c.Member);
                self:write(c.Member.Type.Node.Name);
                self:write(" ");
                self:write(c.Name);               
            end            
        end
    end,
    
    writeInclude = function(self, import)        
        local statement = IncludeFiles.createStatementForNode(import);
        if (statement ~= nil) then self:write(statement); end
    end,
       
    writeTabs = function(self)
        for i = 1, self.tabs do
            self.writer:write('\t');
        end
    end,
    
    writeUsing = function(self, import)             
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
    
    writeVariableInfo = function(self, variable)
        if (variable.Const) then
            self:write("const ");
        end
        if (variable.Pointer) then
            self:write("* ");
        end
        if (variable.Reference) then
            self:write("& ");
        end
        if (variable.ConstPointer) then
            self:write("const ");
        end
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