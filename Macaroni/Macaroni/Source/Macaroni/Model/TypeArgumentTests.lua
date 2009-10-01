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
    self.typeA = self.context.Root:FindOrCreate("TypeA");
    self.typeB = self.context.Root:FindOrCreate("TypeB");
    self.typeC = self.context.Root:FindOrCreate("TypeC");
end

Test.register(
{	
    name = "TypeArgument Tests",    
    tests = {    
        {
            name = "Creating a TypeArgument in Lua with no arguments.",
            init = function(self)
                mixinContext(self);
                self.typeArgument = TypeArgument.New(typeA);
            end,
            tests = {
                ["Node should be what we gave it."] = function(self)
                    Test.assert(self.typeA.FullName, self.typeArgument.Node.FullName);           
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
                local arguments = NodeList.New({self.typeB, self.typeC});           
                self.typeArgument = TypeArgument.New(typeA, arguments);
            end,
            tests = {
                ["Node should be what we gave it."] = function(self)
                    Test.assert(self.typeA.FullName, self.typeArgument.Node.FullName);           
                end,
                ["Arguments are the NodeList we provided."] = function(self)
                    Test.assert(self.typeArguments, self.typeArgument.Arguments);           
                end,
            }
        }
    }    
}); -- End of register call
