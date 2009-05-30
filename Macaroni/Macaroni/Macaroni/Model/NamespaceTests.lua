require "Macaroni.Model.Context";
require "Macaroni.Model.Document";

local Context = Macaroni.Model.Context;
local Document = Macaroni.Model.Document;
local Namespace = Macaroni.Model.Namespace;

Test.register(
{	
name = "Creating a Namespace instance.",
    init = {
        ["Initializing namespaces."] = function(this)
            context = Context.Create("{ROOT}", "*");	    
            root = context.RootNamespace;
            this.a = root:FindOrCreateNamespace("A");
            this.b = this.a:FindOrCreateNamespace("B");
            this.c = this.b:FindOrCreateNamespace("C");       
        end
    },
    
    tests={
        ["Equality method compares by reference, not value."] = function(this)        
            Test.assert(this.b, this.b);	
            Test.assertFalse(this.a, this.b);        
            -- A document establishes its own base node, so the C namespace
            -- it creates will be different than the one above.
            context = Context.Create("{ROOT}", "*");            
            dopplegangerB = context.RootNamespace
                                :FindOrCreateNamespace("A")
                                :FindOrCreateNamespace("B");
            Test.assertFalse(this.b, dopplegangerB);
            Test.assert(tostring(this.b), tostring(dopplegangerB));
        end,
        
        ["FullName property"] = function(this)        
            Test.assert("A::B::C", this.c.FullName);		
        end,
        
        ["IsRoot function"] = function(this)
            Test.assert(false, this.c.IsRoot);
            Test.assert(false, this.a.IsRoot);
            Test.assert(true, this.a.Parent.IsRoot);
        end,
        
        ["Members collection index method"] = function(this)
            members = this.a.Members;
            member1 = members[1];
            Test.assert('A::B', tostring(member1));
        end,
        
        ["Members collection index method throws error on out of bounds"] = function(this)
            local members = this.a.Members;    
            local member0 = members[0];
            Test.assert(nil, member0);
            local member2 = members[2];
            Test.assert(nil, member2);
        end,
        
        ["Members collection length operator"] = function(this)
            members = this.a.Members;
            Test.assert(1, #members);
        end,              
        
        ["Members collection tostring method"] = function(this)
            members = this.a.Members;
            Test.assert('Scope"A" Member Count = 1', tostring(members));
        end,
        
        ["Name property"] = function(this)        
            Test.assert("C", this.c.Name);		
        end,
        
        ["Parent property"] = function(this)        
            Test.assert("B", this.c.Parent.Name);		
        end,
        
        ["Reference counting includes namespaces."] = function(this)        
            context = Context.Create("", "*");
            Test.assert(1, context:GetReferenceCount());        
            a = context.RootNamespace:FindOrCreateNamespace("A");
            collectgarbage("collect"); -- Run garbage collector to ensure we have only two refs.
            Test.assert(2, context:GetReferenceCount()); 
            a = nil;
            collectgarbage("collect"); -- Run garbage collector to ensure we have only 1 refs.
            Test.assert(1, context:GetReferenceCount()); 
        end,   
        
        ["tostring functionality"] = function(this)        
            Test.assert("A::B::C", tostring(this.c));
        end,
    }
});

--[[
Test.register(
{	
name = "Namespace Tests",    
init = {
	["Initializing namespaces."] = function(this)
	    Context c = Context.Create("{ROOT}", "*");	    
	    doc:Read("namespace A::B::C{ }");
	    this.ns = doc.Namespaces[0];        
    end
},

tests={	       
    ["Create Test"] = function(this)
        Context context = Context.Create("{BLAH}", "*");
        root = context.RootNamespace;
        -- Root should have this name.
        Test.assert("{BLAH}", root.Name);
        -- Full name is not blank if its only the root that's being showed.
        Test.assert("{BLAH}", root.FullName);
        -- Child's full name does not include root
        a = root:FindOrCreate("A");
        Test.assert("A", a.FullName);
        b = a:FindOrCreate("B");
        Test.assert("A::B", b.FullName);
        Test.assert("{BLAH}", b.Parent.Parent.Name);      
        Test.assert(b.Parent.Parent, root); 
    end,

    ["Equality method compares by reference, not value."] = function(this)        
		Test.assert(this.ns, this.ns);	
        Test.assertFalse(this.ns, this.ns.Parent);        
        -- A document establishes its own base node, so the C namespace
        -- it creates will be different than the one above.
        docB = Document.New("Test2")
        docB:Read("namespace A::B::C{}");
        nsBizzaroWorldC = docB.Namespaces[0];
        Test.assertFalse(this.ns, nsBizzaroWorldC);        
    end,
    
    ["FullName property"] = function(this)        
		Test.assert("A::B::C", this.ns.FullName);		
    end,
    
    ["Name property"] = function(this)        
		Test.assert("C", this.ns.Name);		
    end,

    ["Parent property"] = function(this)        
		Test.assert("B", this.ns.Parent.Name);		
    end,
    
    ["Reference counting includes namespaces."] = function(this)        
        context = Context.New("", "*");
        Test.assert(0, context.ReferenceCount);        
        context.Root.CreateNamespace("A");
        Test.assert(1, context.ReferenceCount); 
    end,   
    
    ["tostring functionality"] = function(this)        
		Test.assert("A::B::C", tostring(this.ns));
    end,
}

} -- End of Test table.
); -- End of register call
]]--