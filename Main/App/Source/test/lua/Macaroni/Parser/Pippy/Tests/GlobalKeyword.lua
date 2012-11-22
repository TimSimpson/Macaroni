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
require "Macaroni.Model.FileName";
require "Macaroni.Environment.Messages";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.IO.Path";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";

local Access = Macaroni.Model.Cpp.Access;
--local Class = Macaroni.Model.Cpp.Class;
local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local Path = Macaroni.IO.Path;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;
local TypeList = Macaroni.Model.TypeList;
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
name = "PippyParser Tests :: Global Keyword",
tests = {
    {
        name = "~global without paranthesis puts a Node in the level above a class.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            self.file = FileName.Create(Path.New("", "GlobalKeyword.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.library, self.src, [[

				~namespace Something;
				class GlobalKeyword {
					public ~global void Add(int a, int b) { return a + b }
				};
            ]]);
            self.classNode = self.root:Find("Something::GlobalKeyword");
            self.class = self.classNode.Element;
        end,
        tests = {
            ["Class 'GlobalKeyword' gets created in Something namespace."] = function(self)
				local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals("Something::GlobalKeyword", classNode.FullName);
            end,
            ["Class has zero children."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals(0, #classNode.Children);
            end,
            ["Class has 1 global node."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                Test.assertEquals(1, #globals);
            end,
            ["Global node lives in Something."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                local Something1 = globals[1];
                Test.assertEquals("Something::Add::Overload#0", Something1.FullName);
            end,
        }
    },
    {
        name = "~global with paranthesis can define where to put Node.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            self.file = FileName.Create(Path.New("", "GlobalKeyword.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.library, self.src, [[
				~import SomeOtherPlace;
				~namespace Something;

				class GlobalKeyword {
					public ~global(SomeOtherPlace) void Add(int a, int b)
					{ return a + b }
				};
            ]]);
            self.classNode = self.root:Find("Something::GlobalKeyword");
            self.class = self.classNode.Element;
        end,
        tests = {
            ["Class 'GlobalKeyword' gets created in Something namespace."] = function(self)
				local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals("Something::GlobalKeyword", classNode.FullName);
            end,
            ["Class has zero children."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals(0, #classNode.Children);
            end,
            ["Class has 1 global node."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                Test.assertEquals(1, #globals);
            end,
            ["Global node lives in SomeOtherPlace."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                local Something1 = globals[1];
                Test.assertEquals("SomeOtherPlace::Add::Overload#0", Something1.FullName);
            end,
        }
    },

    {
        name = "~global with paranthesis can define the root node with '::'.",
        init = function(self)
            self.parser = PippyParser.Create();
            self.context = Context.New("{ROOT}");
            self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            self.file = FileName.Create(Path.New("", "GlobalKeyword.mcpp"));
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);

            self.parser:Read(self.library, self.src, [[
				~import SomeOtherPlace;
				~namespace Something;

				class GlobalKeyword {
					public ~global(::) void Add(int a, int b)
					{ return a + b }
				};
            ]]);
            self.classNode = self.root:Find("Something::GlobalKeyword");
            self.class = self.classNode.Element;
        end,
        tests = {
            ["Class 'GlobalKeyword' gets created in Something namespace."] = function(self)
				local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals("Something::GlobalKeyword", classNode.FullName);
            end,
            ["Class has zero children."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                Test.assertEquals(0, #classNode.Children);
            end,
            ["Class has 1 global node."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                Test.assertEquals(1, #globals);
            end,
            ["Global node lives in the root namespace."] = function(self)
                local classNode = self.root:Find("Something::GlobalKeyword")
                local globals = classNode.Element.GlobalNodes;
                local Something1 = globals[1];
                Test.assertEquals("Add::Overload#0", Something1.FullName);
            end,
        }
    },

    {
        name = "~global with paranthesis is not allowed if access is ~hidden.",
        init = function(self)
        end,

        tests = {
			['When user tries to use ~hidden and specify node location an error is thrown.']
			= function(self)
				self.parser = PippyParser.Create();
				self.context = Context.New("{ROOT}");
				self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
				self.file = FileName.Create(Path.New("", "GlobalKeyword.mcpp"));
				self.root = self.context.Root;
				self.src = Source.Create(self.file, 1, 1);

				local status, parserException = pcall(function()
					self.parser:Read(self.library, self.src,
					[[  ~import SomeOtherPlace;
						~namespace Something;

						class GlobalKeyword {
							~hidden ~global(::) void Add(int a, int b)
							{ return a + b }
						};
					]]);
					end);
				Test.assertEquals(false, status);
                --TODO: Figure out why ParserException isn't getting glued
                -- to Lua correctly anymore. :(
                --Test.assertEquals(self.file, parserException.Source.FileName);
				--Test.assertEquals(5, parserException.Source.Line);
				self.classNode = self.root:Find("Something::GlobalKeyword");
				self.class = self.classNode.Element;
			end,
		}
	},

} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
