require "Macaroni.Model.Document";

local Document = Macaroni.Model.Document;
local testFile = "TestData/DocumentTests.mcpp";

Test.register(
{	
    name = "Document Tests - Namespaces",
    
--[[init = {
	["Constructing and reading a file"] = function(this)
        this.doc = Macaroni.Model.Document.New(this.testFile);
    end
},

shutdown = {
},]]--
	
tests={	   
    ["Name property works as expected."] = function(this)
		doc = Document.New("Test1");
        Test.assert("Test1", doc.Name);
    end,
    ["Can parse a single namespace."] = function(this)
		doc = Document.New("Test2");
		Test.assert(0, #(doc.Namespaces));
		doc:Read("namespace A { } ");		
		Test.assert(1, #(doc.Namespaces));
		namespaceA = doc.Namespaces[1];
		Test.assert("A", namespaceA.Name);
    end,
    ["Nested namespaces is added correctly."] = function(this)
		doc = Document.New("Test3");
		doc:Read("namespace A::B::C::D { } ");		
		Test.assert(1, #(doc.Namespaces));
		namespaceD = doc.Namespaces[1];
		Test.assert("D", namespaceD.Name);
		Test.assert("C", namespaceD.Parent.Name);
		Test.assert("B", namespaceD.Parent.Parent.Name);
		Test.assert("A", namespaceD.Parent.Parent.Parent.Name);		
		Test.assert("A::B::C::D", namespaceD.FullName);

        doc:Read("namespace A{}");
        Test.assert(2, #(doc.Namespaces));
        namespaceA = doc.Namespaces[1];
        Test.assert("A", namespaceA.Name);        
    end,
    
    ["Namespaces are reopened and not remade."] = function(this)        
        doc = Document.New("test23");
        doc:Read("namespace A::B{}");        
        Test.assert(1, #(doc.Namespaces));
        doc:Read("namespace A::B{}");
        Test.assert(1, #(doc.Namespaces));
	end,
	
    ["Existing namespace instances are used when adding to collection."] = function(this)
        local doc = Document.New("Test3");
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
    end,
    
    ["Can survive comments."] = function(this)
        --pddsfs(); 
        
        doc = Document.New("Test3");
        doc:Read([[
        // Comment.
        namespace A::B::C
        {
        // comment
        }
        // More comments.
        namespace A::B        {
        }
        ]]);
        Test.assert(2, #(doc.Namespaces));
        Test.assert("A::B::C", tostring(doc.Namespaces[1]));
        Test.assert("A::B", tostring(doc.Namespaces[2]));
    end,
    
	--[[["Namespaces are generated correctly."] = function(this)
		Test.assert("3", #(this.file.Namespaces));
	end]]--
}

} -- End of Test table.
);

--[[
Test.register(
{	
    name = "Document Tests - Classes",
    
    init = {
        ["Constructing and reading a file"] = function(this)
            this.doc = Macaroni.Model.Document.New(this.testFile);
        end
    },

shutdown = {
},]]--


