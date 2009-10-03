require "Macaroni.Model.Context";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Model.TypeArgument";

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local TypeArgument = Macaroni.Model.TypeArgument;

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.nodeA = self.context.Root:FindOrCreate("TypeA");
    self.nodeB = self.context.Root:FindOrCreate("TypeB");
    self.nodeC = self.context.Root:FindOrCreate("TypeC");
    self.nodeList = NodeList.New({nodeA, nodeB, nodeC});
end

Test.register(
{	
    name = "TypeArgument Tests",    
    tests = {    
        {
            name = "Creating a TypeArgument in Lua with no arguments.",
            init = function(self)
                mixinContext(self);
                self.typeArgument = TypeArgument.New(self.nodeA);
            end,
            tests = {
                ["Node should be what we gave it."] = function(self)
                    Test.assert(self.nodeA.FullName, self.typeArgument.Node.FullName);           
                end,
                ["Arguments are empty."] = function(self)
                    Test.assert(0, #(self.typeArgument.Arguments));           
                end,
            }
        },
        {
            name = "Creating a TypeArgument in Lua with Arguments.",
            init = function(self)
                mixinContext(self);                     
                self.typeArgument = TypeArgument.New(self.nodeA, self.nodeList);
            end,
            tests = {
                ["Node should be what we gave it."] = function(self)
                    Test.assert(self.nodeA.FullName, self.typeArgument.Node.FullName);           
                end,
                ["Arguments are the NodeList we provided."] = function(self)
                    local expected = self.nodeList;
                    local actual = self.typeArgument.Arguments;
                    Test.assert(expected, actual);
                end,
            }
        }
    }    
}); -- End of register call
