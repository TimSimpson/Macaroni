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
-- require "Macaroni.Model.Cpp.Access";
-- require "Macaroni.Model.Context";
-- require "Macaroni.Model.FileName";
-- require "Macaroni.Environment.Messages";
-- require "Macaroni.Parser.Pippy.PippyParser";
-- require "Macaroni.Parser.Parser";
-- require "Macaroni.Parser.ParserException";
-- require "Macaroni.IO.Path";
-- require "Macaroni.Model.Source";
-- require "Macaroni.Model.Type";
-- require "Macaroni.Model.TypeArgument";
-- require "Macaroni.Model.TypeArgumentList";
-- require "Macaroni.Model.TypeList";

local Access = require "Macaroni.Model.Cpp.Access";
--local Class = require "Macaroni.Model.Cpp.Class";
local Context = require "Macaroni.Model.Context";
local Messages = require "Macaroni.Environment.Messages";
local Path = require "Macaroni.IO.Path";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local TypeArgument = require "Macaroni.Model.TypeArgument";
local TypeArgumentList = require "Macaroni.Model.TypeArgumentList";
local TypeList = require "Macaroni.Model.TypeList";
--local Typedef = Macaroni.Model.Cpp.Typedef;

-- Convience method returns result of the parse, then status, and
-- finally an err message.
local tryParse = function(text)
    local parser = PippyParser.Create();
    local context = Context.New("{ROOT}");
    local file = FileName.Create(Path.New("", "Blah1.mcpp"));
    local src = Source.Create(file, 1, 1);

    local result = nil;
    local status, err = pcall(function()
            result = parser:Read(context, src, text);
        end
    );
    return status, err;
end


Test.register(
{
name = "PippyParser Tests :: Class",
tests = {
    {
        name = "Creating a place holder for a typical C++ dependency.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.target = self.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            self.file = FileName.Create(Path.New("", "a_std.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.target, self.src, [[
                namespace std {
                    class string { ~hfile=<string> }
                }
            ]]);
            self.classNode = self.root.Children[2].Children[1];
            self.class = self.classNode.Element;
        end,
        tests = {
            ["ClassNode has name 'string'."] = function(self)
                Test.assertEquals("string", self.classNode.Name);
            end,
            ["ClassNode has hfile of <string>."] = function(self)
                local actual = self.classNode.HFilePath;
                Test.assertEquals("<string>", tostring(self.classNode.HFilePath));
            end
        }
    },
    {
        name = "Creating an typedef for string.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.target = self.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            self.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.target, self.src, [[
                class Dog
                {
                    public int GetBarkCount(){ return bc; }
                    private int bc;
                    protected:
                        virtual int protectedMethod() = 0;
                        int protectedField1;
                    public char publicField;
                        int protectedField2;

                    Dog()
                    : bc(0),
                      protectedField1(0),
                      protectedField2(0)
                    {}

                    private:
                        int privateField;

                };
            ]]);
            self.classNode = self.root.Children[2];
            self.class = self.classNode.Element;
        end,
        tests = {
            ["ClassNode has name 'Dog'."] = function(self)
                Test.assertEquals("Dog", self.classNode.Name);
            end,
            ["ClassNode has eight children."] = function(self)
                Test.assertEquals(8, #(self.classNode.Children));
            end,
            ["Class has private int."] = function(self)
                local bc = self.classNode.Children[2];
                Test.assertEquals("bc", bc.Name);
                Test.assertEquals(Access.Private, bc.Element.Access);
            end,
            ["Class has public getter."] = function(self)
                local getter = self.classNode.Children[1];
                Test.assertEquals("GetBarkCount", getter.Name);
                Test.assertEquals(Access.Public, getter.Children[1].Element.Access);
            end,
            ["Class has protected method."] = function(self)
                local func = self.classNode.Children[3];
                Test.assertEquals("protectedMethod", func.Name);
                Test.assertEquals(Access.Protected,
                                  func.Children[1].Element.Access);
            end,
            ["Class has protected field 1."] = function(self)
                local field = self.classNode.Children[4];
                Test.assertEquals("protectedField1", field.Name);
                Test.assertEquals(Access.Protected, field.Element.Access);
            end,
            ["Class has public field."] = function(self)
                local field = self.classNode.Children[5];
                Test.assertEquals("publicField", field.Name);
                Test.assertEquals(Access.Public, field.Element.Access);
            end,
            ["Class has protected field 2."] = function(self)
                local field = self.classNode.Children[6];
                Test.assertEquals("protectedField2", field.Name);
                Test.assertEquals(Access.Protected, field.Element.Access);
            end,
            ["Class has protected constructor."] = function(self)
                local ctor = self.classNode.Children[7];
                Test.assertEquals("Constructor", ctor.TypeName);
                Test.assertEquals(Access.Protected,
                                  ctor.Children[1].Element.Access);
            end,
            ["Class has private field."] = function(self)
                local field = self.classNode.Children[8];
                Test.assertEquals("privateField", field.Name);
                Test.assertEquals(Access.Private, field.Element.Access);
            end,

        }
    },
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
