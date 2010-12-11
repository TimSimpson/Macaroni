require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

-- The premise of this class is that each Node's 'light definition' can be 
-- stored in a weak table for quick lookup.
LightDefinition = {
    new = function(self)    
        check(self ~= nil, "Self cannot be nil!");      
        
        setmetatable(self, DependencyList);
        return args;
    end,    
},

LightDefinitions = {
    
    new = function(self)    
        check(self ~= nil, "Self cannot be nil!");      
        
        setmetatable(self, DependencyList);
        self.heavy = {};
        setmetatable(self.heavy, {__mode="k"})
        self.light = {};
        setmetatable(light.heavy, {__mode="k"})
        return args;
    end,    
};