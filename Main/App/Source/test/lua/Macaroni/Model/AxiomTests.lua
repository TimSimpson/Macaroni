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
local Axiom = require "Macaroni.Model.Axiom";

Test.register(
{
    name = "Axiom Tests",
    tests = {
        {
            name = "Creating an Axiom in Lua.",
            init = function(this)
                this.axiom = Axiom.LuaCreate("barf");
            end,
            tests = {
                ["Attempting tostring() on Axiom retrieves words."] = function(this)
                    Test.assertEquals("barf", tostring(this.axiom));
                end,
            }
        }
    }
}); -- End of register call
