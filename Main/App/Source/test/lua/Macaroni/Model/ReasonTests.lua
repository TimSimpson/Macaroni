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
local FileName = require "Macaroni.Model.FileName";
local Path = require "Macaroni.IO.Path";
local Reason = require "Macaroni.Model.Reason";
local Source = require "Macaroni.Model.Source";

Test.register(
{
    name = "Reason tests.",
    tests = {
        {
            name = "Creating a Reason.",
            init = function(this)
                this.file = FileName.Create(Path.New("", "Mino.mcpp"));
                this.source = Source.Create(this.file, 10, 1);
                this.axiom = Axiom.LuaCreate("Class keyword discovered at start of line.");
                this.reason = Reason.Create(this.axiom, this.source);
            end,
            tests = {
                ["Axiom is what was passed in."] = function(this)
                    local a = this.reason.Axiom;
                    Test.assertEquals(this.axiom, a);
                end,
                ["Source is what was passed in."] = function(this)
                    local src = this.reason.Source;
                    Test.assertEquals(this.source, src);
                end,
                ["Tostring concats both relevant parts."] = function(this)
                    Test.assertEquals("Mino.mcpp, line 10, column 1: Class keyword discovered at start of line.",
                                tostring(this.reason));
                end,
            }
        }
    }  -- end of all Reason Tests.
}); -- End of register call
