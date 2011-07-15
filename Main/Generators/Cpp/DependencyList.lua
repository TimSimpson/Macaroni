require "Cpp/Common";
require "Macaroni.Model.Cpp.ClassParent"
require "Macaroni.Model.Cpp.ClassParentList"
if MACARONI_VERSION ~= "0.1.0.20" then
	require "Macaroni.Model.TypeModifiers"
end
require "table"

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;


DependencyTraveller = {
	-- Used in conjunction with a DependencyList. If "heavy" is specified,
	-- it means the dependency is so great the entire C++ definition is
	-- required (in other words, bring out the #include statement).
	-- If not, it means a forward reference will suffice.
	-- However in some contexts even if heavy is false here the Node in question
	-- may require that the "heavy" reference be added. An example would be
	-- something which only uses a pointer to a class which has is defined 
	-- in an external header file unknown to Macaroni. In that case the
	-- #include statement is required no matter what.
	
    heavy = nil,
    originalNode = nil,
    type = "DependencyTraveller",   
    
    new = function(originalNode, heavy)    
        local self = {};
        setmetatable(self, {
			["__index"]=DependencyTraveller,			
			});
        self.originalNode = node; -- who wants this dependency.
        self.heavy = heavy;
        self.type = "DependencyTraveller";
        return self;
    end, 
    
    clone = function(self)
        check(self ~= nil, "Member method called without instance.");
        local c = self.new(self.originalNode, self.heavy);        
        return c;
    end,

    isType = function(obj)
        return type(obj) == "table" and obj.type == "DependencyTraveller";
    end,
    
};

DependencyList = {
	-- Stores a list of dependencies for something like a Node.
    
    new = function(owner)    
        local self = {}
        setmetatable(self, {["__index"]=DependencyList});
        self.heavy = {};
        self.light = {};       
        return self;
    end, 

    addDependenciesForArgumentList = function(self, argumentList, traveller)
        check(self ~= nil, "Missing self.");
        check(argumentList ~= nil, "ArgumentList cannot be nil.");     
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' must be either true or false.");
        for i = 1, #argumentList do
            self:addDependenciesForType(argumentList[i].Member.Type, traveller);            
        end
    end,

    -- Adds the dependency needed by class (iow not the class itself).
    addDependenciesForClass = function(self, node, traveller)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Missing node.");
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' must be DependencyTraveller.");
        check(node.Member ~= nil and node.Member.TypeName == TypeNames.Class, "Node must be Class.");        
        for i = 1, #node.Member.Parents do
			local parent = node.Member.Parents[i];
			local newTraveller = traveller:clone();
			newTraveller.heavy = true; -- need heavy def. for class parent
			self:addDependenciesForType(parent.Parent, newTraveller);
			-- monkey
		end		
        for i = 1, #node.Member.FriendNodes do
            local friend = node.Member.FriendNodes[i];
            if (friend.Member ~= nil and friend.Member.TypeName=="Class" or 
                friend.Member ~= nil and friend.Member.TypeName=="Typedef") then
                --local newTraveller = traveller:clone();
                --newTraveller.heavy = false;
                self:addLightDependencyNode(friend);                
            end            
        end        
        for i = 1, #node.Children do
            self:addDependenciesForNode(node.Children[i], traveller);
        end        
    end,
    
    addDependenciesForConstructor = function(self, member, traveller)
        check(self ~= nil, "Missing self.");
        check(member ~= nil, "Missing member.");
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' must be either true or false.");        
        local overloads = member.Node.Children;
        log:Write("addDependenciesForConstructor " .. tostring(member.Node.FullName))
        for i = 1, #overloads do
        	local olN = overloads[i];
        	local olM = olN.Member;
        	if (olM == nil) then
        		error("Found a nil Member inside of a Constructor:" .. member.Node.Fullname );
    		end    		
    		-- self:addDependenciesForType(olM.ReturnType, traveller); 
        	self:addDependenciesForArgumentList(olM.Arguments, traveller);	
        end        
    end,
    
    addDependenciesForFunction = function(self, member, traveller)
        check(self ~= nil, "Missing self.");
        check(member ~= nil, "Missing member.");
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' cannot be nil.");
        local overloads = member.Node.Children;
        for i = 1, #overloads do
        	local olN = overloads[i];
        	local olM = olN.Member;
        	if (olM == nil) then
        		error("Found a nil Member inside of a Function:" .. member.Node.Fullname );
    		end    		
    		self:addDependenciesForType(olM.ReturnType, traveller); 
        	self:addDependenciesForArgumentList(olM.Arguments, traveller);	
    	end        
    end,
    
    -- Adds dependencies necessary for this node.
    -- The entry point this be.
    addDependenciesForNode = function(self, node)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Missing node.");
        --check(heavy == true or heavy == false,
        --      'Argument #3, "heavy", must be true or false.');        
        if (node.Member ~= nil) then
            self:addDependenciesForNodeMember(node, node.Member);
        end
    end,
    
    -- Adds dependencies needed by the given node & member.
    addDependenciesForNodeMember = function(self, node, member)
        check(self ~= nil, "Missing self.");
        check(member ~= nil, "Missing member.");
        --check(traveller ~= nil, "Argument 3 'traveller' must be either true or false.");
        if (member.TypeName == TypeNames.Variable) then              
            self:addDependenciesForType(member.Type, 
                                        DependencyTraveller.new(node, true));            
        end
        if (member.TypeName == TypeNames.Class) then
            self:addDependenciesForClass(node, 
                                         DependencyTraveller.new(node, false));
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Function) then      
            self:addDependenciesForFunction(member, 
                                            DependencyTraveller.new(node, true));             
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Constructor) then     
            self:addDependenciesForConstructor(member, 
                                               DependencyTraveller.new(node, true));
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Typedef) then
            -- Typedefs always attempt light dependencies
            self:addDependenciesForType(member.Type, DependencyTraveller.new(node, false));
        end
    end, 
           
    -- Adds the node itself to the light dependencies list.
    addDependencyNode = function(self, node, traveller)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Node cannot be nil.");
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' must be either true or false.");
        if (traveller.heavy) then
            addHeavyDependencyNode(node);
        else
            addLightDependencyNode(node);
        end
    end,
    
    -- Adds the given type itself to the dependencies
    addDependenciesForType = function(self, type, traveller, asLight)
        check(self ~= nil, "Missing self.");
        check(type ~= nil, "Type cannot be nil.");    
        check(DependencyTraveller.isType(traveller), "Argument 3' must be DependencyTraveller.");
        
        asLight = asLight or false
        -- ^- This is an experimental thing. The idea is that 
        -- boost::shared_ptr<Blah> appears everywhere, but "Blah" is 
        -- interpretted (correctly) as something that must include a whole 
        -- #include file which stinks! But if its light, that isn't necessary...
        
        local newTraveller = traveller:clone();
        if MACARONI_VERSION=="0.1.0.20" then
			if not asLight then        
				if (type.Pointer or type.ConstPointer or type.Reference) then
					newTraveller.heavy = false;  
				end
			else
				newTraveller.heavy = false;
			end
		else
			if type.Modifiers.MayOnlyNeedForwardDeclaration then
				newTraveller.heavy = false;
			end
		end
        if (not self:canUseLightDef(type.Node)) then
            -- HFiles refer to something defined elsewhere we're including.
            -- A light definition is fundamentally impossible, so force heavy def.
            newTraveller.heavy = true;
        end
                
        check(type.Node ~= nil, "Given a type with a nil node!");
        self:addDependencyNode(type.Node, newTraveller);     
        
        -- this is a hack, but if it works, I'll add real support
        -- for some kind of keyword or something into Macaroni
        local light = type.Node.FullName == "boost::shared_ptr"
                      or type.Node.Fullname == "boost::intrusive_ptr";
                                  
        if (type.TypeArguments ~= nil) then
            for i = 1, #type.TypeArguments do
                local typeArg = type.TypeArguments[i];
                for j = 1, #typeArg.Arguments do
                    local typeArgType = typeArg.Arguments[j];                                        
                    self:addDependenciesForType(typeArgType, traveller, light); 
                end                
            end
        end
    end,
        
    -- Adds the node itself to the light dependencies list.
    addDependencyNode = function(self, node, traveller)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Node cannot be nil.");
        check(DependencyTraveller.isType(traveller),
              "Argument 3 'traveller' must be DependencyTraveller.");
        if (node.Member == nil) then
            error("addDependencyNode was passed " .. node.FullName .. ", which is a hollow node..", 2);            
        end  
        -- This was called so the Node really HAS to be a type, or there's an error...
        local member = node.Member;
        if (member.TypeName == TypeNames.Class) then
            if (traveller.heavy) then 
                self:addHeavyDependencyNode(node);
            else
                self:addLightDependencyNode(node);           
            end
        elseif (member.TypeName == TypeNames.Primitive) then
            -- Ignore! :D
        elseif (member.TypeName == TypeNames.Typedef) then
            -- The typedef itself always gets a light dependency.
            self:addLightDependencyNode(node); 
            -- But then we have to iterate its underlying types also.
            self:addDependenciesForType(member.Type, traveller);
        else
            error("addDependencyNode was passed " .. tostring(node.FullName) .. ", which is not a type but " .. node.Member.TypeName .. ".", 2);       
        end        
    end,
    
    -- Adds the node itself to the heavy dependencies list.
    addHeavyDependencyNode = function(self, node)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Node cannot be nil.");
        self.heavy[node.FullName] = node;
    end,       
    
     -- Adds the node itself to the heavy dependencies list.
    addLightDependencyNode = function(self, node)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Node cannot be nil.");
        self.light[node.FullName] = node;
    end,   
    
    canUseLightDef = function(self, node)
		-- Light definitions are preferable but sometimes Macaroni can't use 
		-- them. For example, if the definition comes from an external file.
		if node.HFilePath == nil then
			return true;
		end
		local attr = node.Annotations["Macaroni::Cpp::UseLightDef"];			
		if (attr ~= nil) then
			if not attr.IsBool then
				error("The node " .. node.FullName .. " annotation value "
				      .. "UseLightDef must be a boolean.");
			end
			if (attr.ValueAsBool) then
				return true;
			end
		end
		return false;
    end,    
    
    iterateDependencies = function(self, t)
		local nodes = {}
		for k, v in pairs(t) do
			nodes[#nodes + 1] = v
		end
		table.sort(nodes, function (a, b)
			if a.Name == b.Name then		  
				return string.lower(a.FullName) < string.lower(b.FullName)
			else
				return string.lower(a.Name) < string.lower(b.Name)
			end
		end)
		local i = 0
		return function()
			i = i + 1
			if i <= #nodes then return nodes[i] end
		end
    end,
    
    iterateHeavyDependencies = function(self)
		return self:iterateDependencies(self.heavy);
    end,
    
    iterateLightDependencies = function(self)
		return self:iterateDependencies(self.light);		
    end,
        
};
