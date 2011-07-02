--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------
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