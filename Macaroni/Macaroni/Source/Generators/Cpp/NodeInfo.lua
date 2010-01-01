require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

-- Stores quick lookup info about Nodes.
NodeInfo = {
    lightDef = nil,
    using = nil,
    
    new = function(node)    
        check(node ~= nil, "Node cannot be nil!");      
        local self = {}
        setmetatable(self, NodeInfo);       
        self.node = node;
        self.lightDef = NodeInfo.createLightDef(self.node);
        self.using = NodeInfo.createUsingStatement(self.node);
        return self;
    end,    
    
    -- Creates the light definition, in the form of a String. Includes newlines.
    createLightDef = function(node)
        check(node ~= nil, "Argument one must be node.");
        if (node.IsRoot) then
            return ""; -- Ignore
        end
        local generateWarning = true;
        if (node.Member ~= nil) then            
            if (node.Member.TypeName == TypeNames.Class) then
                return 'class ' .. node.FullName .. ';\n';   
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return ""; -- Ignore
            end            
        end
        return "/* ~ <(I don't know how to generate definition for " .. node.FullName .. ".) */\n"; 
    end,
    
    -- Creates using statement.
    createUsingStatement = function(node)
        check(node ~= nil, "Argument one must be node.");
        if (node.IsRoot) then
            return ""; -- Ignore
        end
        local generateWarning = true;
        if (node.Member ~= nil) then            
            if (node.Member.TypeName == TypeNames.Class
                or node.Member.TypeName == TypeNames.Typedef) then
                return 'using ' .. node.FullName .. ';\n';   
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return ""; -- Ignore
            end            
        end
        return "/* ~ <(I don't know how to generate a using statement for " .. node.FullName .. ".) */\n";        
    end,
    
};

NodeInfoList = {
    instance = nil;
    
    new = function()    
        local self = {}
        setmetatable(self, NodeInfoList);       
        self.nodes = {};
        setmetatable(self.nodes, { __mode="k" }); 
        -- fyi: "__mode"=="kv" will make the values weak as well; a good idea if this ends up hogging memory.
        return self;
    end, 
    
    get = function()
        if NodeInfoList.instance == nil then
            NodeInfoList.instance = NodeInfoList.new();
        end
        return NodeInfoList.instance;
    end,
    
    __index = function(self, key)
        local rtn = self.nodes[key];
        if (rtn == nil) then
            rtn = NodeInfo.new(key);
            self.nodes[key] = rtn;
        end
        return rtn;
    end,
    
};

NodeInfoList = NodeInfoList.new();