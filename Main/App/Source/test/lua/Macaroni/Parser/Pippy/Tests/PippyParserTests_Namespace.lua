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
local Messages = require "Macaroni.Environment.Messages";
local Path = require "Macaroni.IO.Path";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Source = require "Macaroni.Model.Source";

Test.register(
{
name = "PippyParser Tests :: Namespace",
tests = {
     {
        name = "Passing blank through parser.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.target = this.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.target, this.src, "");
        end,
        tests = {
            ["One node exists within the Context"] = function(this)
                Test.assertEquals(1, #this.root.Children);
            end,
            ["Node is for primitives."] = function(this)
                Test.assertEquals("{C++ Primitives}", this.root.Children[1].Name);
            end,
        }
    },

    {
        name = "Reading a single namespace.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.target = this.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.target, this.src, [[
                namespace Apple { }
            ]]);
        end,
        tests = {
            ["Only two nodes are found within the Context"] = function(this)
                -- This unit test always fails, but its due to some condition caused
                -- by a prior unit test.  I can make another method just like it and it
                -- passes.
                Test.assertEquals(2, #this.root.Children);
            end,
            ["Only two nodes are found within the Context 2"] = function(this)
                Test.assertEquals(2, #this.root.Children);
            end,
            ["Second node in Root is Apple."] = function(this)
                Test.assertEquals("Apple", this.root.Children[2].Name);
            end,
            ["Apple namespace has no children."] = function(this)
                Test.assertEquals(0, #this.root.Children[2].Children);
            end,
            ["Type of Node is Namespace."] = function(this)
                Test.assertEquals("Namespace", this.root.Children[2].Element.TypeName);
            end,
        }
    },

    {
        name = "Reading a TWO namespaces.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.target = this.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.target, this.src, [[
                namespace Apple {
                    namespace Seed
                    {
                    }
                }
            ]]);

        end,
        tests = {
            ["Node within node found within the Context"] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals(1, #this.root.Children[2].Children);
            end,
            ["Single node in Root is Apple, then Seed."] = function(this)
                Test.assertEquals("Apple", this.root.Children[2].Name);
                Test.assertEquals("Seed", this.root.Children[2].Children[1].Name);
            end,
            ["Seed namespace has no children."] = function(this)
                Test.assertEquals(0, #this.root.Children[2].Children[1].Children);
            end,
        }
    },

    {
        name = "Namespace parsing throws errors on invalid conditions.",

        -- Convience method returns result of the parse, then status, and
        -- finally an err message.
        tryParse = function(text)
            local parser = PippyParser.Create();
            local context = Context.New("{ROOT}");
            local target = context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            local file = FileName.Create(Path.New("", "Blah1.mcpp"));
            local src = Source.Create(file, 1, 1);

            local result = nil;
            local status, err = pcall(function()
                    result = parser:Read(target, src, text);
                end
            );
            return status, err;
        end,

        init = function(this)
        end,

        tests = {
            --TODO: Figure out why ParserException isn't being parsed correctly!
            -- ["Garbage appears before Namespace."] = function(this)
            --     local status, err = this.tryParse("dsjfk");
            --     Test.assertEquals(false, status);
            --     Test.assertEquals(1, err.Source.Line);
            --     Test.assertEquals(1, err.Source.Column);
            --     Test.assertEquals(Messages.Get("CppParser.Variable.UnknownTypeName"), err.Message);
            -- end,
            -- ["No identifier after namespace keyword."] = function(this)
            --     local status, err = this.tryParse("namespace {}");
            --     Test.assertEquals(false, status);
            --     Test.assertEquals(1, err.Source.Line);
            --     Test.assertEquals(11, err.Source.Column);
            --     Test.assertEquals(Messages.Get("CppParser.Namespace.NoID1"), err.Message);
            -- end,
            -- ["Junk appears between namespace identifier and braces."] = function(this)
            --     local status, err = this.tryParse("namespace sa::fsd /12 {}");
            --     Test.assertEquals(false, status);
            --     Test.assertEquals(1, err.Source.Line);
            --     Test.assertEquals(19, err.Source.Column);
            --     Test.assertEquals(Messages.Get("CppParser.Namespace.NoOpeningBrace"), err.Message);
            -- end,
            -- ["Inside of braces has junk."] = function(this)
            --     local status, err = this.tryParse("namespace sa::fsd { fkfk}");
            --     Test.assertEquals(false, status);
            --     Test.assertEquals(1, err.Source.Line);
            --     Test.assertEquals(21, err.Source.Column);
            --     Test.assertEquals(Messages.Get("CppParser.Variable.UnknownTypeName", 1), err.Message);
            -- end,
        }
    },

} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
