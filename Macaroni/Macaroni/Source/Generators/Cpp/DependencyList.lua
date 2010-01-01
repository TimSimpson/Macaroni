require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

DependencyList = {
    
    new = function(self)    
        check(self ~= nil, "Self cannot be nil!");      
        
        setmetatable(args, DependencyList);
        self.heavy = {};
        self.light = {};       
        return args;
    end, 

     
};