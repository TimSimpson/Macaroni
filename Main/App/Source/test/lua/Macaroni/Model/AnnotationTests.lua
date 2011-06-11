require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.AttributeTable";
require "Macaroni.Model.AttributeValue";
require "Macaroni.Model.Context";
require "Macaroni.Model.Node";

local Access = Macaroni.Model.Cpp.Access;
local AttributeTable = Macaroni.Model.AttributeTable;
local AttributeValue = Macaroni.Model.AttributeValue;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;


Test.register(
{	
name = "Attribute Tests",    
tests = {    
    {   name = "Creating a node with some attributes.",
        init = function(this)
            this.context = Context.New("{r}");
            this.root = this.context.Root;
            this.newNode = this.root:FindOrCreate("Dog");
            this.attr = this.newNode.Attributes;
        end,
        tests = {
            ["Find returns the same Node that was created."] = function(this)
                local foundNode = this.root:Find("Dog");
                Test.assert(this.newNode, foundNode);
            end,            
        }        
    },    
} -- end of tests
}); -- End of register call
