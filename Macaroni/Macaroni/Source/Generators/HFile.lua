require "Macaroni.Generator.Output.Path";
require "Macaroni.Model.Context";
require "io";

-- Eventually put this next part in C++.
Macaroni.Model.TypeNames = { Class = "Class", Namespace = "Namespace"};

local TypeNames = Macaroni.Model.TypeNames;

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;

print "Hello, I am dumb.\n";

function Generate(context, path)
    print "Generating H Files\n";
    
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
    local filePath = path:NewPath(".h");
    local cg = ClassGenerator.new{node = node, path = filePath};
    cg:parse();
end

function parseNamespace(node, path)
    assert(node.Member.TypeName == TypeNames.Namespace);    
    iterateNodes(node.Children, path);
end

function parseNode(node, path)
    print("~~~ " .. node.FullName);
    local m = node.Member;
    if (m == nil) then
        return;
    end
    local typeName = m.TypeName;
    print("       " .. typeName);
    local newPath = path:NewPath("/" .. node.Name);
    
    local handlerFunc = nil;
    if (typeName == TypeNames.Namespace) then
        handlerFunc = parseNamespace;
    elseif (typeName == TypeNames.Class) then
        handlerFunc = parseClass;
    end
    
    if (handlerFunc ~= nil) then
        handlerFunc(node, newPath);
    else
        print("     ~ Skipping");
    end 
end


ClassGenerator = {
   
    new = function(args)
    
        assert(args.path ~= nil);
        assert(args.node ~= nil);
    
        --TODO: Somehow calling C++ NewFileWriter() method currently results in entire Lua call from C++ ending.
        local writer, errorMsg, errorNumber = io.open(args.path.AbsolutePath, 'w+'); --args.path:NewFileWriter(); 
        if (writer == nil) then
            print(tostring(errorNumber) .. " " .. errorMsg);
        end
        args.writer = writer;
        setmetatable(args, ClassGenerator);
        ClassGenerator.__index = ClassGenerator;
        return args;
    end,
    
    classBegin = function(self)
        self.writer:write("class " .. self.node.Name .. "\n");
        self.writer:write("{\n");
    end,
    
    classBody = function(self)
        self:classBegin();
        self:iterateClassMembers(self.node.Children);
        self:classEnd();
    end,
    
    classEnd = function(self)        
        self.writer:write("}\n");
    end,
    
    getGuardName = function(self)
        local guardName = "MACARONI_COMPILE_GUARD_" .. self.node:GetPrettyFullName("/");
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
        -- alas, this is impossible.
        -- The NodePtrList type needs LuaGlue so the property can be accessed from the Class member.
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
        for i, v in pairs(names) do
            self.writer:write("namespace " .. v .. " { ");
        end
        self.writer:write("\n");
    end,
    
    namespaceEnd = function(self)
        local names = Node.SplitComplexName(self.node.FullName);
        for i,v in pairs(names) do
            self.writer:write("} ");
        end
        self.writer:write("\n");
    end,
    
    parse = function(self)          
        self:includeGuardHeader();
        self.writer:write('\n');
        self:includeStatements();
        self.writer:write('\n');
        self:namespaceBegin();
        self.writer:write('\n');
        self:classBody();
        self.writer:write('\n');
        self:namespaceEnd();
        self.writer:write('\n');
        self:includeGuardFooter();
    end,
    
    parseMember = function(self, node)
        self.writer:write(node.Name .. '\n');
    end,
};