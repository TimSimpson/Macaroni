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
require "Macaroni.Model.Axiom";
require "Macaroni.Model.Cpp.Class";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Library";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.IO.Path";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";

local Access = Macaroni.Model.Cpp.Access;
local Axiom = Macaroni.Model.Axiom;
local Class = Macaroni.Model.Cpp.Class;
local Context = Macaroni.Model.Context;
local FileName = Macaroni.Model.FileName;
local Library = Macaroni.Model.Library;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local Path = Macaroni.IO.Path;
local Reason = Macaroni.Model.Reason;
local Source = Macaroni.Model.Source;

Test.register(
{
name = "Class Tests",
tests = {
    {   name = "Creating a class by hand",
        init = function(this)
            this.context = Context.New("{r}");
            this.library = this.context.RootLibrary;
            this.root = this.context.Root;
            this.newNode = this.root:Find("Dog");
            Test.assertEquals(nil, this.newNode);
            this.newNode = this.root:FindOrCreate("Dog");
            Test.assertNotEquals(nil, this.newNode);
            this.imports = NodeList.New{};
            this.reason = nil;
            this.axiom = Axiom.LuaCreate("LuaGlue.LuaClassWraps");
            this.fileName = FileName.Create(Path.New("", "MadeUpFile"));
            this.src = Source.Create(this.fileName, 3, 5);
            this.reason = Reason.Create(this.axiom, this.src);
            this.newClass = Class.Create(this.library, this.newNode,
                                         Access.Public, this.imports,
                                         this.reason);
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
            ["The Class can use Element methods such as Library."] = function(this)
                local class = this.newClass;
                local library = class.Library;
                Test.assertEquals(this.library, library);
            end,
            ["The Class can use Element methods such as TypeName."] = function(this)
                local class = this.newClass;
                local typename = class.TypeName;
                Test.assertEquals("Class", tostring(typename));
            end,
            ["The Class can use Class methods such as ImportedNodes."] = function(this)
                local class = this.newClass;
                local importedNodes = class.ImportedNodes;
                Test.assertEquals(0, #importedNodes);
            end,
            --["The default of a node is private."] = function(this)
            --    local foundNode = this.root:Find("Dog");
            --    local expected = Access.Private;
            --    local actual = foundNode.Access;
            --    Test.assertEquals(expected, actual);
            --end,
        }
    },   -- end of test 1
} -- tests
}); -- End of register call
