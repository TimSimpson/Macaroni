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
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Path = require "Macaroni.IO.Path";
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
name = "PippyParser Tests :: Templates",
tests = {
    {
        name = "Simple template function.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.target = self.context:Group("Tests")
                :Project("Test"):Version("1.0"):Target("hi");
            self.file = FileName.Create(Path.New("", "Template1.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.target, self.src, [[
                template<typename T>
                int foo(int i)
                {
                    return i = i + 1;
                }
            ]]);
        end,
        tests = {
            ["A function is created."] = function(self)
                Test.assertEquals(2, #self.root.Children);
                Test.assertEquals("Function",
                                  self.root.Children[2].Element.TypeName);
            end,
            ["The function has a Node for the template parameter list."]
            = function(self)
                local func = self.root.Children[2]
                local fOl = func.Children[1]
                Test.assertEquals(2, #fOl.Children);
                Test.assertEquals("$t", fOl.Children[1].Name);
                Test.assertEquals("foo::Overload#0::$t", fOl.Children[1].FullName);
                local tpl = fOl.Children[1];
                Test.assertEquals(1, #tpl.Children);
                Test.assertEquals("T", tpl.Children[1].Name);

            end,
        }
    },
} -- end of tests table
}); -- End of register call
