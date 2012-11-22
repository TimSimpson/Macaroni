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
require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.Context";
require "Macaroni.Model.Node";

local Access = Macaroni.Model.Cpp.Access;
local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;


Test.register(
{
name = "Node Tests",
tests = {
    {   name = "Creating a node with some children by hand.",
        init = function(this)
            this.context = Context.New("{r}");
            this.root = this.context.Root;
            this.newNode = this.root:Find("Dog");
            Test.assertEquals(nil, this.newNode);
            this.newNode = this.root:FindOrCreate("Dog");
            Test.assertNotEquals(nil, this.newNode);
        end,
        tests = {
            ["Find returns the same Node that was created."] = function(this)
                local foundNode = this.root:Find("Dog");
                Test.assertEquals(this.newNode, foundNode);
            end,
            ["The Children property has the same reference."] = function(this)
                local nodes = this.root.Children;
                Test.assertEquals(1, #nodes);
                Test.assertEquals(foundNode, nodes[0]);
            end,
            --["The default of a node is private."] = function(this)
            --    local foundNode = this.root:Find("Dog");
            --    local expected = Access.Private;
            --    local actual = foundNode.Access;
            --    Test.assertEquals(expected, actual);
            --end,
        }
    },

    {
        name = "When parsing complex names",
        tests={
            ["ParseComplexName creates nothing if given a blank."] = function(this)
                local context = Context.New("{ROOT}", "{WILDCARD}");
                local resultNode, resultName =
                    context.Root:ParseComplexName("");
                Test.assertEquals("", resultName);
                Test.assertEquals(0, #context.Root.Children);
            end,

            ["ParseComplexName creates nothing if given a simple name."] = function(this)
                local context = Context.New("{ROOT}", "{WILDCARD}");
                local resultNode, resultName = context.Root:ParseComplexName("Doggy");
                Test.assertEquals("Doggy", resultName);
                Test.assertEquals(0, #context.Root.Children);
            end,

            ["ParseComplexName creates UnknownScope if given a complex name."] = function(this)
                local context = Context.New("{ROOT}", "{WILDCARD}");
                local resultNode, resultName = context.Root:ParseComplexName("Animals::Doggy");
                Test.assertEquals("Doggy", resultName);
                Test.assertEquals(1, #(context.Root.Children));
                Test.assertEquals("Animals", resultNode.Name);
                Test.assertEquals(context.Root.Children[1], resultNode);
                Test.assertEquals(0, #(resultNode.Children));
            end,

            ["ParseComplexName creates multiple UnknownScope if needed."] = function(this)
                local context = Context.New("{ROOT}", "{WILDCARD}");
                local resultNode, resultName =
                    context.Root:ParseComplexName("Organisms::Animals::Kitty");
                Test.assertEquals("Kitty", resultName);
                Test.assertEquals(1, #(context.Root.Children));
                local organisms = context.Root.Children[1];
                Test.assertEquals("Organisms", organisms.Name);
                Test.assertEquals(1, #(organisms.Children));
                Test.assertEquals("Animals", context.Root.Children[1].Children[1].Name);
                Test.assertEquals(0, #(context.Root.Children[1].Children[1].Children));
                Test.assertEquals(context.Root.Children[1].Children[1], resultNode);
                Test.assertEquals("Organisms::Animals", resultNode.FullName);
            end,

        }
    }, -- end complex names

    {
        name = "Node Tests-Static Helper Functions",

        tests={

            ["IsComplexName returns false for blanks."] = function(this)
                Test.assertEquals(false, Node.IsComplexName(""));
            end,

            ["IsComplexName returns false for simple names."] = function(this)
                Test.assertEquals(false, Node.IsComplexName("A"));
                Test.assertEquals(false, Node.IsComplexName("Animal"));
                Test.assertEquals(false, Node.IsComplexName("Doggy42"));
            end,

            ["IsComplexName returns true in the presense of ::."] = function(this)
                Test.assertEquals(true, Node.IsComplexName("::"));
                Test.assertEquals(true, Node.IsComplexName("A::B"));
                Test.assertEquals(true, Node.IsComplexName("Organisms::Animals::Dog"));
            end,

            ["IsSimpleName returns true for blanks."] = function(this)
                Test.assertEquals(true, Node.IsSimpleName(""));
            end,

            ["IsSimpleName returns true for simple names."] = function(this)
                Test.assertEquals(true, Node.IsSimpleName("A"));
                Test.assertEquals(true, Node.IsSimpleName("Doggy"));
                Test.assertEquals(true, Node.IsSimpleName("DogCatDemonHide"));
            end,

            ["IsSimpleName returns false when faced with ::."] = function(this)
                Test.assertEquals(false, Node.IsSimpleName("A::B"));
                Test.assertEquals(false, Node.IsSimpleName("Animals::Doggy"));
                Test.assertEquals(false, Node.IsSimpleName("Ryu::Enemies::DogCatDemonHide"));
            end,

            ["SplitComplexName returns component parts."] = function(this)
                local names = Node.SplitComplexName("boost::filesystem::path");
                Test.assertEquals(3, #names);
                Test.assertEquals("boost", names[1]);
                Test.assertEquals("filesystem", names[2]);
                Test.assertEquals("path", names[3]);
            end,

            ["SplitFirstNameOffComplexName returns nothing for nothing."] = function(this)
                local firstPart, lastPart = Node.SplitFirstNameOffComplexName("")
                Test.assertEquals("", firstPart);
                Test.assertEquals("", lastPart);
            end,

            ["SplitFirstNameOffComplexName returns only first part when no :: are found."] = function(this)
                local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Doggy")
                Test.assertEquals("Doggy", firstPart);
                Test.assertEquals("", lastPart);
            end,

            ["SplitFirstNameOffComplexName returns simple string when some stuff after :: is found."] = function(this)
                local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Animal::Doggy")
                Test.assertEquals("Animal", firstPart);
                Test.assertEquals("Doggy", lastPart);
            end,

            ["SplitFirstNameOffComplexName returns lots of stuff if found after ::."] = function(this)
                local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Organisms::Animal::Doggy")
                Test.assertEquals("Organisms", firstPart);
                Test.assertEquals("Animal::Doggy", lastPart);
            end,

            ["SplitNodeAndMemberName scope is blank, member is blank if nothing given."] = function(this)
                local scope, member = Node.SplitNodeAndMemberName("")
                Test.assertEquals("", scope);
                Test.assertEquals("", member);
            end,

            ["SplitNodeAndMemberName scope is blank if no :: found."] = function(this)
                local scope, member = Node.SplitNodeAndMemberName("a")
                Test.assertEquals("", scope);
                Test.assertEquals("a", member);
            end,

            ["SplitNodeAndMemberName works with one :: found."] = function(this)
                local scopeName, memberName = Node.SplitNodeAndMemberName("b::a")
                Test.assertEquals("b", scopeName);
                Test.assertEquals("a", memberName);
            end,

            ["SplitNodeAndMemberName works with two :: found."] = function(this)
                local scopeName, memberName = Node.SplitNodeAndMemberName("Arg::big::Orange")
                Test.assertEquals("Arg::big", scopeName);
                Test.assertEquals("Orange", memberName);
            end,

            ["SplitNodeAndMemberName works with three (or lets presume more) :: found."] = function(this)
                local scopeName, memberName = Node.SplitNodeAndMemberName("King::Darius::Gradius::Vic")
                Test.assertEquals("King::Darius::Gradius", scopeName);
                Test.assertEquals("Vic", memberName);
            end,

            ["SplitComplexName returns zero elements for blank."] = function(this)
                local names = Node.SplitComplexName("")
                Test.assertEquals({}, names);
            end,

            ["SplitComplexName returns one element for single names."] = function(this)
                local names = Node.SplitComplexName("Porcupine")
                Test.assertEquals({"Porcupine"}, names);
            end,

             ["SplitComplexName returns correctly for C++ primitives."] = function(this)
                local names = Node.SplitComplexName("")
                Test.assertEquals({}, names);
            end,

            ["SplitComplexName returns 2 elements when it needs to."] = function(this)
                local names = Node.SplitComplexName("Animals::Porcupine")
                Test.assertEquals({"Animals", "Porcupine"}, names);
            end,

            ["SplitComplexName returns 3 or more elements when it needs to."] = function(this)
                local names = Node.SplitComplexName("Organisms::Animals::Porcupine")
                Test.assertEquals({"Organisms", "Animals", "Porcupine"}, names);
            end,
        }
    } -- End of static tests
}
}); -- End of register call
