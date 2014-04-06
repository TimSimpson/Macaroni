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
-- require "Macaroni.Model.Context";
-- require "Macaroni.Environment.Messages";
-- require "Macaroni.Parser.Pippy.PippyParser";
-- require "Macaroni.Parser.Parser";
-- require "Macaroni.Parser.ParserException";
-- require "Macaroni.Model.Source";

local Context = require "Macaroni.Model.Context";
local Messages = require "Macaroni.Environment.Messages";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Source = require "Macaroni.Model.Source";


Test.register(
{
name = "PippyParser Tests :: Import",
tests = {
     {
        name = "Importing stuff adds it.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah1.mcpp");
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.context, this.src, [[
                import System::Graphics::BlahBlahBlah;
                import Turtles::Fight::With::Honor;
                import Cat;
            ]]);
        end,
        tests = {
            ["The imports are added as maleable nodes."] = function(this)
                Test.assertEquals(4, #this.root.Children);
                Test.assertEquals("{C++ Primitives}", this.root.Children[1].Name);
                Test.assertEquals("System", this.root.Children[2].Name);
                Test.assertEquals("Turtles", this.root.Children[3].Name);
                Test.assertEquals("Cat", this.root.Children[4].Name);
            end,
            ["Sub nodes are also added."] = function(this)
                Test.assertEquals("Turtles::Fight::With::Honor",
                            this.root.Children[3].Children[1].Children[1].FullName);
            end,
        }
    },



} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
