require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.AnnotationTable";
require "Macaroni.Model.AnnotationValue";
require "Macaroni.Model.Context";
require "Macaroni.Model.Node";

local Access = Macaroni.Model.Cpp.Access;
local AnnotationTable = Macaroni.Model.AnnotationTable;
local AnnotationValue = Macaroni.Model.AnnotationValue;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;


Test.register(
{	
name = "Annotation Tests",    
tests = {    
    {   name = "Creating a node with some attributes.",
        init = function(this)
            this.context = Context.New("{r}");
            this.root = this.context.Root;
            this.newNode = this.root:FindOrCreate("Dog");
            this.attr = this.newNode.Annotations;
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
