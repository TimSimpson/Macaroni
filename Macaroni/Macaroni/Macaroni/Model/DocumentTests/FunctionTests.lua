require "Macaroni.Model.Document";

-- These are for the stand alone tests.
local Document = Macaroni.Model.Document;

Test.register(
{	
    name = "Document Tests - Functions",
    
--[[init = {
	["Constructing and reading a file"] = function(this)
        this.doc = Macaroni.Model.Document.New(this.testFile);
    end
},

shutdown = {
},]]--
	
tests={	      
    ["Simple function."] = function(this)
        --pddsfs();         
        doc = Document.New("SimpleMain");
        doc:Read([[
        // Comment.
        namespace A::B::C
        {
            void DoSomething()
            {
                std::cout << "Hello, EARTH!";
            }
        // comment
        }       
        ]]);
        Test.assert(1, #(doc.Namespaces));
        Test.assert("A::B::C", tostring(doc.Namespaces[0]));
        abc = Namespaces[0];
        func = abc.GetFunctions(0);
        func.
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


