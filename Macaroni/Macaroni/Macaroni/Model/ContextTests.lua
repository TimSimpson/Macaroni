require "Macaroni.Model.Context";
require "Macaroni.Model.Namespace";
require "Macaroni.Model.Scope";

local Context = Macaroni.Model.Context;
local Namespace = Macaroni.Model.Namespace;
local Scope = Macaroni.Model.Scope;

Test.register(
{	
name = "Context Tests",    
tests={	       
    ["CreateRoot Test"] = function(this)
        this.context = Context.New("{ROOT}", "{WILDCARD}");        
        Test.assert("{ROOT}", tostring(context.RootNamespace));
        Test.assert("{WILDCARD}", tostring(context.WildcardNamespace));
    end,
    
    
    ["ParseComplexName will morph unknown Scopes into Namespaces."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local d = context.RootNamespace:FindOrCreateNamespace("A::B::C::D");
        Test.assert("A::B::C::D", d.FullName);       
        local d_c = d.Scope;
        Test.assert("A::B::C", d_c.FullName);
        local d_b = d_c.Scope;
        Test.assert("A::B", d_b.FullName);
        local d_a = d_b.Scope;
        Test.assert("A", d_a.FullName);       
        Test.assert(1, #(context.RootNamespace.Members));
       
        -- Must reuse A, and morph it into an Namespace...
        -- I.. am.. morphing...
        local a = context.RootNamespace:FindOrCreateNamespace("A");
        Test.assert("A", a.FullName);
        Test.assertFalse(a, d_a);
        Test.assert(1, #(context.RootNamespace.Members));
        Test.assert("Good", "popo in pants");
    end,
        
    --[[
    ["Existing namespace instances are used when adding to collection."] = function(this)        
        local context = Context.New("{r}", "*");
        Scope.
context.RootNamespace.
        
		doc:Read("namespace A::B::C::D { } ");
		doc:Read("namespace A{}");
		Test.assert(2, #(doc.Namespaces));
		local a = doc.Namespaces[2];
		local d = doc.Namespaces[1];
		Test.assert("A::B::C::D", d.FullName);
		Test.assert("A", a.FullName);		
		local c = d.Parent;
		local b = d.Parent.Parent;		
		local aReference2 = d.Parent.Parent.Parent;
		Test.assert(a, aReference2);
    end,]]--
    
    ["Root and wildcard namespaces created properly"] = function(this)
        context = Context.New("{ROOT}", "{WILDCARD}");
        root = context.RootNamespace;
        Test.assert("{ROOT}", tostring(root));        
        wildcard = context.WildcardNamespace;
        Test.assert("{WILDCARD}", tostring(wildcard));        
    end,
    
    ["Can correctly discover Members within a Context."] = function(this)
        context = Context.New("{ROOT}", "{WILDCARD}");
        root = context.RootNamespace;
        --root:
        error("To-DO");
    end
}

} -- End of Test table.
); -- End of register call
