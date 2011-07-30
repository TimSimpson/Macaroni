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
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeList";

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeList = Macaroni.Model.TypeList;

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.nodeA = self.context.Root:FindOrCreate("TypeA");
    self.nodeB = self.context.Root:FindOrCreate("TypeB");
    self.nodeC = self.context.Root:FindOrCreate("TypeC");
    self.typeA = Type.New(self.nodeA);
    self.typeB = Type.New(self.nodeB);
    self.typeC = Type.New(self.nodeC);
    self.typeList = TypeList.New{self.typeA, self.typeB, self.typeC};
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
                    Test.assertEquals(self.nodeA.FullName, self.typeArgument.Node.FullName);           
                end,
                ["Arguments are empty."] = function(self)
                    Test.assertEquals(0, #(self.typeArgument.Arguments));           
                end,
            }
        },
        {
            name = "Creating a TypeArgument in Lua with Arguments.",
            init = function(self)
                mixinContext(self);                     
                self.typeArgument = TypeArgument.New(self.nodeA, self.typeList);
            end,
            tests = {
                ["Node should be what we gave it."] = function(self)
                    Test.assertEquals(self.nodeA.FullName, self.typeArgument.Node.FullName);           
                end,
                ["Arguments are the TypeList we provided."] = function(self)
                    local expected = self.typeList;
                    local actual = self.typeArgument.Arguments;
                    Test.assertEquals(expected, actual);
                end,
            }
        }
    }    
}); -- End of register call
