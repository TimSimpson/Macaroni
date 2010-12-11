require "Cpp/Common";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local TypeNames = Macaroni.Model.TypeNames;

DependencyTraveller = {
    heavy = nil,
    originalNode = nil,
    type = "DependencyTraveller",
    
    new = function(originalNode, heavy)    
        local self = {};
        setmetatable(self, {["__index"]=DependencyTraveller});              
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
        self:addDependenciesForArgumentList(member.Arguments, traveller);
    end,
    
    addDependenciesForFunction = function(self, member, traveller)
        check(self ~= nil, "Missing self.");
        check(member ~= nil, "Missing member.");
        check(DependencyTraveller.isType(traveller), "Argument 3 'traveller' cannot be nil.");
        self:addDependenciesForType(member.ReturnType, traveller); 
        self:addDependenciesForArgumentList(member.Arguments, traveller);
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
            self:addDependenciesForType(member.Type, DependencyTraveller.new(node, true));            
        end
        if (member.TypeName == TypeNames.Class) then
            self:addDependenciesForClass(node, DependencyTraveller.new(node, false));
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Function) then      
            self:addDependenciesForFunction(member, DependencyTraveller.new(node, true));             
        end
        if (member.TypeName == Macaroni.Model.TypeNames.Constructor) then     
            self:addDependenciesForConstructor(member, DependencyTraveller.new(node, true));
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
    addDependenciesForType = function(self, type, traveller)
        check(self ~= nil, "Missing self.");
        check(type ~= nil, "Type cannot be nil.");    
        check(DependencyTraveller.isType(traveller), "Argument 3  3mq' must be DependencyTraveller.");
        local newTraveller = traveller:clone();
        if (type.Pointer or type.ConstPointer or type.Reference) then
            newTraveller.heavy = false;  
        end
        if (type.Node.HFilePath ~= nil) then
            -- HFiles refer to something defined elsewhere we're including.
            -- A light definition is fundamentally impossible, so force heavy def.
            newTraveller.heavy = true;
        end
        check(type.Node ~= nil, "Given a type with a nil node!");
        self:addDependencyNode(type.Node, newTraveller);                               
        if (type.TypeArguments ~= nil) then
            for i = 1, #type.TypeArguments do
                local typeArg = type.TypeArguments[i];
                for j = 1, #typeArg.Arguments do
                    local typeArgType = typeArg.Arguments[j];
                    self:addDependenciesForType(typeArgType, traveller); 
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
        self.heavy[node] = true;
    end,       
    
     -- Adds the node itself to the heavy dependencies list.
    addLightDependencyNode = function(self, node)
        check(self ~= nil, "Missing self.");
        check(node ~= nil, "Node cannot be nil.");
        self.light[node] = true;
    end,   
    
    
};
