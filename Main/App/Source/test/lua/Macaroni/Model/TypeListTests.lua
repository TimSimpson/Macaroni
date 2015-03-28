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
local Context = require "Macaroni.Model.Context";
local FileName = require "Macaroni.Model.FileName";
local Messages = require "Macaroni.Environment.Messages";
local Node = require "Macaroni.Model.Node";
local NodeList = require "Macaroni.Model.NodeList";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local TypeArgumentList = require "Macaroni.Model.TypeArgumentList";
local TypeList = require "Macaroni.Model.TypeList";

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.stdstring = self.context.Root:FindOrCreate("std::string");
    self.stdstringType = Type.New(self.stdstring);
end

Test.register(
{
    name = "TypeList Tests",
    tests = {
        {
            name = "Creating an empty TypeList.",
            init = function(self)
                self.typeList = TypeList.New{};
            end,
            tests = {
                ["TypeList is empty."] = function(self)
                    Test.assertEquals(0, #(self.typeList));
                end,
            }
        },
        {
            name = "Creating a TypeList with one element.",
            init = function(self)
                mixinContext(self);
                self.typeList = TypeList.New{self.stdstringType};
            end,
            tests = {
                ["TypeList has one element."] = function(self)
                    Test.assertEquals(1, #(self.typeList));
                end,
                ["TypeList first element is what we gave it."] = function(self)
                    Test.assertEquals(self.stdstringType, self.typeList[1]);
                end,
            }
        },
    }
}); -- End of register call
