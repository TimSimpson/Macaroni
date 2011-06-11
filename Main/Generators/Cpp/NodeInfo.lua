require "Macaroni.Model.AnnotationTable"
require "Macaroni.Model.AnnotationValue";
require "Cpp/Common";
require "Cpp/DependencyList";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

-- Stores quick lookup info about Nodes.
NodeInfo = {
    dependencies = nil,
    headerFile = nil,
    heavyDef = nil,
    lightDef = nil,
    using = nil,
    
    new = function(node)    
        check(node ~= nil, "Node cannot be nil!");      
        local self = {}
        setmetatable(self, {["__index"]=NodeInfo});       
        self.node = node;         
        self.dependencies = self:createDependencyList(self.node);
        self.headerFile = self:createHeaderFile(self.node);
        self.heavyDef = self:createHeavyDef(self.node);
        self.lightDef = self:createLightDef(self.node);
        self.using = self:createUsingStatement(self.node);
        return self;
    end,    
    
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
            node.Member.TypeName == Macaroni.Model.TypeNames.Primitive) then
            return "";
        end
        local path = nil;
        if (node.HFilePath ~= nil) then 
            return tostring(node.HFilePath);
        else
            if (node.AdoptedHome ~= nil) then
                return self:createHeaderFile(node.AdoptedHome);
            end
            if (node.Member ~= nil and node.Member.TypeName == Macaroni.Model.TypeNames.Class) then
                return '<' .. node:GetPrettyFullName("/") .. '.h>'; 
            else
                return '<' .. node.Node:GetPrettyFullName("/") .. '/_.h>';
            end            
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
    
    beginNs =  function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local fs = namespaceNode.FullName;
        local rtn = "";
        if not namespaceNode.IsRoot then
			local names = Macaroni.Model.Node.SplitComplexName(fs);        
			for i = 1, #names do
				rtn = rtn .. "namespace " .. names[i] .. " { ";
			end
			rtn = rtn .. "\n";
		end
        return rtn;
    end,    
    
    endNs = function(self, namespaceNode)
        check(namespaceNode ~= nil, "namespaceNode cannot be nil.");
        local rtn = "";
        if not namespaceNode.IsRoot then
			local names = Macaroni.Model.Node.SplitComplexName(namespaceNode.FullName);        
			for i = 1, #names do
				rtn = rtn .. "} ";
			end
			rtn = rtn .. "// End namespace ";
			rtn = rtn .. "\n";
		end
		return rtn;		
    end,   
    
    -- Creates the light definition, in the form of a String. Includes newlines.
    createLightDef = function(self, node)    
        check(self ~= nil, "Member method called without self.");
        check(node ~= nil, "Argument one must be node.");        
        if (node.IsRoot or node.HFilePath ~= nil) then
			ignore = true
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
				return ""; -- Ignore
			end
        end
        local generateWarning = true;
        if (node.Member ~= nil) then            
            if (node.Member.TypeName == TypeNames.Class) then
                local rtn = self:beginNs(node.Node);
                rtn = rtn ..  'class ' .. node.Name .. ';\n';   
                rtn = rtn .. self:endNs(node.Node);
                return rtn;
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return ""; -- Ignore
            elseif (node.Member.TypeName == TypeNames.Typedef) then
                local typeUtil = TypeUtil.new();
                local rtn = self:beginNs(node.Node);
                rtn = rtn ..  'typedef ' .. 
                            typeUtil:createTypeDefinition(node.Member.Type, false) ..
                            ' ' .. node.Name .. ';';   
                rtn = rtn .. self:endNs(node.Node);
                return rtn;               
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
                local rtn = 'using ' .. node.FullName .. ';\n';   
                return rtn;
            elseif (node.Member.TypeName == TypeNames.Primitive) then
                return ""; -- Ignore
            end            
        end
        return "// ~ <(I don't know how to generate a using statement for " .. node.FullName .. ".) \n";        
    end,
    
    useLightDef = function(self, node)
		-- Gives an answer as to whether the light def can be used for this 
		-- node. If there's no header file, we have no choice- we HAVE to use 
		-- it, becuase we're generating it anyway and there's no reason not to.
		-- If we have a header file but "UseLightDef" is defined, we should
		-- use the light def anyway.
		if node.HFilePath == nil then
			return true;
		end
		local attr = node.Annotations["Macaroni::Cpp::UseLightDef"];			
		if (attr ~= nil) then
			if not attr.IsBool then
				error("The node " .. node.FullName .. " annotation value "
					  .. "UseLightDef must be a boolean.");
			end
			return attr.ValueAsBool;
		end	
		return false;
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