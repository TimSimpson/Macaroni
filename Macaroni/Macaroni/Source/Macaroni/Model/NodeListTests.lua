require "Macaroni.Model.Context";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.nodeA = self.context.Root:FindOrCreate("TypeA");
    self.nodeB = self.context.Root:FindOrCreate("TypeB");
    self.nodeC = self.context.Root:FindOrCreate("TypeC");
end

Test.register(
{	
    name = "NodeList Tests",    
    tests = {    
        {
            name = "Creating an empty NodeList in Lua.",
            init = function(self)
                self.nodeList = NodeList.New({});
            end,
            tests = {
                ["Result is empty."] = function(self)
                    Test.assert(0, #(self.nodeList));
                end,
            }
        },
        {
            name = "Creating a NodeList with one value in Lua.",
            init = function(self)
                mixinContext(self);
                self.nodeList = NodeList.New({self.nodeA});
            end,
            tests = {
                ["Result has one element."] = function(self)
                    Test.assert(1, #(self.nodeList));
                end,
                ["The element, when retrieved, is what we put in originally."] = function(self)
                    Test.assert(self.nodeA, self.nodeList[1]);
                end,
            }
        },
        {
            name = "Creating a NodeList with two values in Lua.",
            init = function(self)
                mixinContext(self);
                self.nodeList = NodeList.New({self.nodeA, self.nodeB});
            end,
            tests = {
                ["Result has two elements."] = function(self)
                    Test.assert(2, #(self.nodeList));
                end,
                ["The elements can be retrieved via indexing."] = function(self)
                     Test.assert(self.nodeA, self.nodeList[1]);
                     Test.assert(self.nodeB, self.nodeList[2]);
                end,
            }
        },
    }    
}); -- End of register call
