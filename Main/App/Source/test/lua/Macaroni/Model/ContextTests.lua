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
require "Macaroni.Model.Context";
--require "Macaroni.Model.Namespace";
require "Macaroni.Model.Node";

local Context = Macaroni.Model.Context;
local Namespace = Macaroni.Model.Namespace;
local Node = Macaroni.Model.Node;

Test.register(
{	
name = "Context Tests",    
tests={	       
    ["CreateRoot Test"] = function(this)
        this.context = Context.New("{ROOT}");        
        Test.assert("{ROOT}", tostring(this.context.Root));        
    end,   
    
    ["ToString returns a string of some kind."] = function(this)
        local lContext = Context.New("{ROOT}");        
        Test.assert("Context[references:3,Root:{ROOT}]", tostring(lContext));        
    end,
    
    
    ["ParseComplexName will morph unknown Nodes into Namespaces."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local d = context.Root:FindOrCreate("A::B::C::D");
        Test.assert("A::B::C::D", d.FullName);       
        local d_c = d.Node;
        Test.assert("A::B::C", d_c.FullName);
        local d_b = d_c.Node;
        Test.assert("A::B", d_b.FullName);
        local d_a = d_b.Node;
        Test.assert("A", d_a.FullName);       
        Test.assert(1, #(context.Root.Children));
       
        -- Must reuse A, and morph it into an Namespace...
        -- I.. am.. morphing...
        local a = context.Root:FindOrCreate("A");
        Test.assert("A", a.FullName);
        Test.assert(a, d_a);
        Test.assert(1, #(context.Root.Children));
    end,
        
    --[[
    ["Existing namespace instances are used when adding to collection."] = function(this)        
        local context = Context.New("{r}", "*");
        Node.
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
        context = Context.New("{ROOT}");
        root = context.Root;
        Test.assert("{ROOT}", tostring(root));                
    end,    
}

} -- End of Test table.
); -- End of register call
