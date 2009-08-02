require "Macaroni.Model.Class";
require "Macaroni.Model.Namespace";

local Class = Macaroni.Model.Class;
local Namespace = Macaroni.Model.Namespace;

--[[
Test.register(
{	
name = "Class Tests",    
init = {
	["Initializing root namespace."] = function(this)
	    Namespace = 	    
	    doc = Document.New("Test1");
	    doc:Read("namespace A::B::C{ }");
	    this.ns = doc.Namespaces[0];        
    end
},

tests={	       
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
    
    ["tostring functionality"] = function(this)        
		Test.assert("A::B::C", tostring(this.ns));
    end,
}

} -- End of Test table.
); -- End of register call]]--

