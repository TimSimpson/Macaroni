require "Macaroni.Model.Context";
require "Macaroni.Model.UnknownScope";

local Context = Macaroni.Model.Context;
local UnknownScope = Macaroni.Model.UnknownScope;

Test.register(
{	
name = "A Scope is initially unknown.",
    init = {
        ["Initializing Contexts."] = function(this)
            this.context = Context.Create("{ROOT}", "*");	    
            this.root = context.RootNamespace;
            this.a = root:FindOrCreate("A");
            this.b = this.a:FindOrCreate("B");
            this.c = this.b:FindOrCreate("C");
             // Oh yeah baby, TWO return parms!  Hellz yeah, I'm a hustla.
            this.resultScope, this.simpleName = 
                this.context.root.ParseComplexName(this.context.root,
                                                   "A::B::C::D::AClassInD");
        end
    },
    
    tests={      
        ["Simple name is correct"] = function(this)                    
            Test.assert("AClassInD", this.simpleName);
        end,
        ["The result Scope is equal to what is obtained by traversing Namespaces manually."] = function(this)
            Test.assert("A::B::C::D", this.resultScope.FullName);
            Test.assert(this.C.Members[0], this.resultScope);
        end,
        ["Result Scope does not have Class methods."] = function(this)
            // resultScope should be a Namespace.    
            Test.assert(nil, this.resultScope.TypeName);            
        end,
        ["Initially, the class we added is an unknown type as well."] = function(this)
            local AClassInD = this.resultScope.Members[0];            
            // We intend for this to eventually become a Class...
            Test.assert("A::B::C::D::AClassInD", AClassInD);
            // But without Type Info, Macaroni can't assume that.
            Test.assert(nil, AClassInD.TypeName);
        end,      
    }
});



Test.register(
{	
name = "A UnknownScope can morph into a known type of Scope.",
    init = {
        ["Initializing Contexts."] = function(this)
            this.context = Context.Create("{ROOT}", "*");	    
            this.root = context.RootNamespace;
            this.a = root:FindOrCreate("A");
            this.b = this.a:FindOrCreate("B");
            this.c = this.b:FindOrCreate("C");
            this.resultScope, this.simpleName = 
                this.context.root.ParseComplexName(this.context.root,
                                                   "A::B::C::D::AClassInD");
            this.ScopeA = this.resultScope.Members[0];
            // Have to make this into a Class now...
            this.AClassInD = this.resultScope.FindOrCreateClass("AClassInD");           
            // At this point, we should never again be able to access the UnknownScope object...
            this.ScopeB = this.resultScope.Members[0]; 
        end
    },
    
    tests={      
        ["Simple name is correct"] = function(this)                    
            Test.assert("AClassInD", this.UnknownScopeAClassInD);
            Test.assert("AClassInD", this.KnownScopeAClassInD);
        end,
        
        ["Class properties are accessible through the discovered Class."] = function(this)
            Test.asset(nil, this.ScopeA.TypeName);
            Test.assert("A::B::C::D::AClassInD", this.AClassInD);
            Test.assert("A::B::C::D::AClassInD", this.ScopeB);          
        end,              
    }
});


Test.register(
{	
name = "A known scope cannot morph again.",
    init = {
        ["Initializing Contexts."] = function(this)
            this.context = Context.Create("{ROOT}", "*");	    
            this.root = context.RootNamespace;
            this.a = root:FindOrCreate("A");
            this.b = this.a:FindOrCreate("B");
            this.c = this.b:FindOrCreate("C");
            this.resultScope, this.simpleName = 
                this.context.root.ParseComplexName(this.context.root,
                                                   "A::B::C::D::AClassInD");
            this.ScopeA = this.resultScope.Members[0];
            // Have to make this into a Class now...
            this.AClassInD = this.resultScope.FindOrCreateClass("AClassInD");           
            // At this point, we should never again be able to access the UnknownScope object...
            this.ScopeB = this.resultScope.Members[0]; 
        end
    },
    
    tests={      
        ["Simple name is correct"] = function(this)                    
            Test.assert("AClassInD", this.UnknownScopeAClassInD);
            Test.assert("AClassInD", this.KnownScopeAClassInD);
        end,
        
        ["Class properties are accessible through the discovered Class."] = function(this)
            Test.asset(nil, this.ScopeA.TypeName);
            Test.assert("A::B::C::D::AClassInD", this.AClassInD);
            Test.assert("A::B::C::D::AClassInD", this.ScopeB);          
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