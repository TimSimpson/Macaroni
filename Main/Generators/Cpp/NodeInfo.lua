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
require "Macaroni.Model.AnnotationTable"
require "Macaroni.Model.AnnotationValue";
require "Cpp/Common";
require "Cpp/DependencyList";

local Access = require "Macaroni.Model.Cpp.Access";
local Context = require "Macaroni.Model.Context";
local Node = require "Macaroni.Model.Node";
local TypeNames = Macaroni.Model.TypeNames;

-- Short for "NodeDefinition". These can be heavy or light.
--
-- "heavy" means including the definition causes a bit of heartburn; in other
--  words, it means the header file is #include'd.
-- "light" means its a forward declaration, like "class A;" or something.
--
-- In some cases a "light" definition is for a hollow node, not created with
-- Macaroni, in which case it ends up being a header file include anyway.
--
DependencyUseObserver = {
    notify = function(target, nodeInfo, heavy, text)
        -- Do nothing
    end
}


-- Stores quick lookup info about Nodes.
NodeInfo = {
	-- This information is difficult enough that it should live inside Macaroni
	-- native code.  It defines things about the node needed for C++
	-- generation, such as where the header file lives, what the "light"
	-- definition (a forward reference) looks like, what the heavy one
	-- (an #include statement) looks like, etc.

	dependencies = nil,
    headerFile = nil,
    _heavyDef = nil,
    _lightDef = nil,
    _lightIsHeavy = nil,
    using = nil,

    new = function(node)
        check(node ~= nil, "Node cannot be nil!");
        local self = {}
        setmetatable(self, {["__index"]=NodeInfo});
        self.node = node;
        self.dependencies = self:createDependencyList(self.node);
        self.headerFile = self:createHeaderFile(self.node);
        self._heavyDef = self:createHeavyDef(self.node);
        self._lightDef, self._lightIsHeavy, self._lightDefNoNs
            = self:createLightDef(self.node);
        self.using = self:createUsingStatement(self.node);
        self.useHeavyDef = function(target)
            DependencyUseObserver.notify(target, self, true, self._heavyDef)
            return self._heavyDef
        end
        self.useLightDef = function(target)
            -- Nothing uses this so let's disable it for now.
            DependencyUseObserver.notify(target, self, self._lightIsHeavy, self._lightDef)
            return self._lightDef
        end
        self.useLightDefNoNs = function(target)
            return self._lightDefNoNs;
        end
        return self;
    end,

    beginNs =  function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local fs = namespaceNode.FullName;
        local rtn = "";
        if not namespaceNode.IsRoot then
			local names = Node.SplitComplexName(fs);
			for i = 1, #names do
				rtn = rtn .. "namespace " .. names[i] .. " { ";
			end
			rtn = rtn .. "\n";
		end
        return rtn;
    end,

    -- Only in 0.1.0.20
    createDependencyList = function(self, node)
        -- Functions and variables don't currently get these because their
        -- info is too dependent on context.
        check(self ~= nil, "Member function called without self.");
        check(node ~= nil, "Node cannot be nil.");
        local list = DependencyList.new();
        list:addDependenciesForNode(node);
        return list;
    end,

    createHeaderFile = function(self, node)
        check(self ~= nil, "Member function called without self.");
        check(node ~= nil, "Argument 2, 'node', cannot be nil.");
        if (node.Member ~= nil and
            (node.Member.TypeName == TypeNames.Primitive
             or node.Member.TypeName == "AnnotationDefinition")
            ) then
            return "";
        end
        local path = nil;
        if (node.HFilePath ~= nil) then
            return tostring(node.HFilePath);
        else
            if (node.AdoptedHome ~= nil) then
                return self:createHeaderFile(node.AdoptedHome);
            elseif self:isUnderClass(node) then
				return self:createHeaderFile(node.Node);
            elseif (node.Member ~= nil) then
                if MACARONI_VERSION ~= "0.1.0.27" then
                    local target = node.Element.Owner;
                    if (target ~= nil and target.HFile ~= nil) then
                        return '<' .. target.HFile.RelativePathNormalized .. '>';
                    end
                end
				if (node.Member.TypeName == TypeNames.Class or
				    node.Member.TypeName == TypeNames.Typedef)
				    then
					return '<' .. node:GetPrettyFullName("/") .. '.h>';
				end
			end
            return '<' .. node.Node:GetPrettyFullName("/") .. '/_.h>';
        end
        return "";
    end,

    -- Creates the heavy definition- the include statement.
    createHeavyDef = function(self, node)
        check(self ~= nil, "Member function called without self.");
        check(node ~= nil, "Argument 2, 'node', cannot be nil.");
        local headerFilePath = self:createHeaderFile(node);
        if (headerFilePath == "") then
            return "";
        else
            return ('#include ' .. headerFilePath .. '\n');
        end
    end,

    endNs = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local rtn = "";
        if not namespaceNode.IsRoot then
			local names = Node.SplitComplexName(namespaceNode.FullName);
			for i = 1, #names do
				rtn = rtn .. "} ";
			end
			rtn = rtn .. "// End " .. namespaceNode.FullName;
			rtn = rtn .. "\n";
		end
		return rtn;
    end,

    -- Creates the light definition, in the form of a String. Includes newlines.
    createLightDef = function(self, node)
        check(self ~= nil, "Member method called without self.");
        check(node ~= nil, "Argument one must be node.");
        if (node.IsRoot) then
			return "", false, ""; -- Ignore
        end
        if (node.HFilePath ~= nil) then
            local ignore = true;
            local attr = node.Annotations["Macaroni::Cpp::LightDef"];
            if (attr ~= nil) then
                if not attr.IsString then
                    error("The node " .. node.FullName .. " annotation value "
                          .. "LightDef must be a string.");
                end
                return attr.ValueAsString, false, attr.ValueAsString;
            end
			local attr = node.Annotations["Macaroni::Cpp::UseLightDef"];
			if (attr ~= nil) then
				if not attr.IsBool then
					error("The node " .. node.FullName .. " annotation value "
					      .. "UseLightDef must be a boolean.");
				end
				if (attr.ValueAsBool) then
					ignore = false;
				end
			end
			if ignore then
				-- For header files are forced to use this... :(
				return self:createHeavyDef(node), true, self:createHeavyDef(node);
			end
        end
        local generateWarning = true;
        if (node.Member ~= nil) then
            if (node.Member.TypeName == TypeNames.Class) then
                if self:isUnderClass(node) then
                    -- Use the parent Node's light def instead.
                    node = self:findParentClass(node)
                end

                local bigRtn = self:beginNs(node.Node);
                local rtn = '';
                WriteTemplateParameterList(
                    node.Element,
                    function(arg)
                        rtn = rtn .. arg;
                    end
                );
				rtn = rtn ..  'class ' .. node.Name .. ';\n';
                bigRtn = self:beginNs(node.Node) .. rtn ..self:endNs(node.Node);

                return bigRtn, false, rtn;
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return "", false, ""; -- Ignore
            elseif (node.Member.TypeName == TypeNames.Typedef) then
				return self:createHeavyDef(node), true;
                --local typeUtil = TypeUtil.new();
                --local rtn = self:beginNs(node.Node);
                --rtn = rtn ..  'typedef ' ..
                            --typeUtil:createTypeDefinition(node.Member.Type, false) ..
                            --' ' .. node.Name .. ';';
                --rtn = rtn .. self:endNs(node.Node);
                --return rtn;
            end
        end
        return "// ~ <(I don't know how to generate definition for " .. node.FullName .. ".)\n";
    end,

    -- Creates using statement.
    createUsingStatement = function(self, node)
        check(self ~= nil, "Member method called without self.");
        check(node ~= nil, "Argument one must be node.");
        if (node.IsRoot or node.Node.IsRoot) then
            return ""; -- Ignore
        end
        local generateWarning = true;
        if (node.Member ~= nil) then
            if (node.Member.TypeName == TypeNames.Class
                or node.Member.TypeName == TypeNames.Typedef) then
                if (node.Node ~= nil and node.Node.TypeName == TypeNames.Class)
                then
                    return "// ~ <(Skipping using statement for nested class "
                        .. node.FullName .. ".)\n";
                end
                local rtn = 'using ' .. node.FullName .. ';\n';
                return rtn;
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return ""; -- Ignore
            end
        end
        return "// ~ <(I don't know how to generate a using statement for " .. node.FullName .. ".) \n";
    end,

    isUnderClass = function(self, node)
		local parent = node.Node;
		if (parent ~= nil and parent.Member ~= nil) then
			if parent.Member.TypeName == TypeNames.Class then
				return true;
			end
        end
    end,

    findParentClass = function(self, node)
        while(node.Node ~= nil and node.Node.TypeName == TypeNames.Class) do
            node = node.Node
        end
        return node
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
        check(key ~= nil, "Node cannot be nil!");
        local keyName = key.FullName;
        local rtn = self.nodes[keyName];
        if (rtn == nil) then
            rtn = NodeInfo.new(key);
            self.nodes[keyName] = rtn;
        end
        return rtn;
    end,

};

NodeInfoList = NodeInfoList.new();
