require "Cpp/NodeInfo";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

-- The start of a C++ file includes dependency information.
-- This class takes as input dependencies.
-- It stores them in a list, with the dependencies whom depend on others
-- least at the top, such that the dependencies will be in order.
DependencySection = {
    new = function(owner)    
        local self = {}
        setmetatable(self, {["__index"]=DependencySection});
        self.list = {};               
        return self;
    end, 
    
    add = function(self, node)       
        local info = NodeInfoList[node];
        local d = info.dependencies;
        for k,v in pairs(d.light) do
            self:addToList(k);
        end
        for k,v in pairs(d.heavy) do
            self:addToList(k);
        end
    end,   
    
    addToList = function(self, newNode)
        local lIndex = self:findListIndexOfLastDependency(NodeInfoList[newNode].dependencies.light);        
        local hIndex = self:findListIndexOfLastDependency(NodeInfoList[newNode].dependencies.heavy);
        --if (lIndex < hIndex) then
            -- Only do both if hIndex appears later in dependency list.
            table.insert(self.list, lIndex, {node = newNode, heavy = false});
        --end
        table.insert(self.list, hIndex, {node = newNode, heavy = true});
    end,
        
    eraseDuplicates = function(self)
        for i = 1, #self.list do
            local index = self:indexOfDependency(self.list[i], i + 1);
            if (index ~= nil) then
                table.remove(self.list, i);
                i = i - 1;
            end
        end
    end,    
    
    -- Searches list for the first occurance of any elements from dependencyList.
    findListIndexOfLastDependency = function(self, dependencyList)
        for i = #self.list, 1, -1 do -- todo: confirm iteration is as expected
            local listElement = self.list[i];
            for k, v in pairs(dependencyList) do
                if (k == listElement.node) then
                    return i + 1;      
                end
            end     
        end       
        return 1;
    end,
    
    indexOfDependency = function(self, dependency, startIndex)
        startIndex = startIndex or 1;
        for i = startIndex, #self.list do
            if (self.list[i].node == dependency.node and
                self.list[i].heavy == dependency.heavy) then
                return i;
            end
        end
        return nil;
    end,
};