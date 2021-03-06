require "Cpp/NodeInfo";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
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
        for node in d:iterateLightDependencies() do
            self:addToList(node, false);
        end
        for node in d:iterateHeavyDependencies() do
            self:addToList(node, true);
        end
        if node.TypeName == "FunctionOverload"
           or node.TypeName == "ConstructorOverload" then
           self:add(node.Node);
        end
    end,

    addToList = function(self, newNode, isHeavy)
        self.list[#self.list + 1] = {node = newNode, heavy = isHeavy}
    end,

    -- I think this method is broken and useless...
    OLDaddToList = function(self, newNode)
        local lIndex = self:findListIndexOfLastDependency(NodeInfoList[newNode].dependencies.light);
        local hIndex = self:findListIndexOfLastDependency(NodeInfoList[newNode].dependencies.heavy);
        --if (lIndex < hIndex) then
            -- Only do both if hIndex appears later in dependency list.
            table.insert(self.list, lIndex, {node = newNode, heavy = false});
        --end
        table.insert(self.list, hIndex, {node = newNode, heavy = true});
    end,

    eraseDuplicates = function(self)
		local i = 1
		while i < #self.list do
            local index = 0;
            repeat
				if self.list[i] == nil then
					local m = ''
					for j = 1, #self.list do
						m = m .. j .. " = " .. tostring(self.list[j]) .. '\n'
					end
					error("self.list[" .. tostring(i) .. "] == nil! List= "
					      .. m)
				end
				index = self:indexOfDependency(self.list[i], i + 1);
				if (index ~= nil) then
					table.remove(self.list, index);
				end
			until index == nil
			i = i + 1
			if i >= #self.list then
				return
			end
        end
    end,

    eraseNode = function(self, node)
		local index;
		repeat
			index = self:indexOfNode(node);
			if (index ~= nil) then
				table.remove(self.list, index);
			end
		until index == nil
    end,

    -- Searches list for the first occurance of any elements from dependencyList.
    findListIndexOfLastDependency = function(self, dependencyList)
        for i = #self.list, 1, -1 do -- todo: confirm iteration is as expected
            local listElement = self.list[i];
            for k, v in pairs(dependencyList) do
                if (v == listElement.node) then
                    return i + 1;
                end
            end
        end
        return 1;
    end,

    indexOfDependency = function(self, dependency, startIndex)
		if (dependency == nil) then
			error("Arg #2, dependency, can't be nil!")
		end
        startIndex = startIndex or 1;
        for i = startIndex, #self.list do
            if (self.list[i].node.FullName == dependency.node.FullName and
                self.list[i].heavy == dependency.heavy) then
                return i;
            end
        end
        return nil;
    end,

    indexOfNode = function(self, node)
		for i = 1, #self.list do
            if (self.list[i].node.FullName == node.FullName) then
                return i;
            end
        end
        return nil;
    end,
};
