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


