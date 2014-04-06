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
-- require "Macaroni.IO.Path";
-- require "Macaroni.Model.Source";
-- require "Macaroni.Model.Type";
-- require "Macaroni.Model.TypeArgument";
-- require "Macaroni.Model.TypeArgumentList";
-- require "Macaroni.Model.TypeList";

local Context = require "Macaroni.Model.Context";
local Messages = require "Macaroni.Environment.Messages";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local Path = require "Macaroni.IO.Path";
local FileName = require "Macaroni.Model.FileName";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local TypeArgument = require "Macaroni.Model.TypeArgument";
local TypeArgumentList = require "Macaroni.Model.TypeArgumentList";
local TypeList = require "Macaroni.Model.TypeList";

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
name = "PippyParser Tests :: Functions",
tests = {
    {
        name = "Creating an empty method.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "PippyParserTests_Functions", "9.5");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                void main(){}
            ]]);
            this.funcNode = this.root.Children[2];
            this.func = this.funcNode.Element;
            this.fo1 = this.funcNode.Children[1].Element;
        end,
        tests = {
            ["Main is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("main", this.root.Children[2].Name);
            end,
            ["Main's type is function."] = function(this)
                Test.assertEquals("Function", this.func.TypeName);
            end,
            ["Main has one overload."] = function(this)
				Test.assertEquals(1, #(this.funcNode.Children));
            end,
            ["Main.Overload#0 type is overload."] = function(this)
				Test.assertEquals("FunctionOverload", this.fo1.TypeName);
            end,
            ["Main.Overload#0 return type is void."] = function(this)
                Test.assertEquals("void", this.fo1.ReturnType.Node.Name);  --
            end,
            ["Main.Overload#0 argument list is empty."] = function(this)
                Test.assertEquals(0, #this.fo1.Arguments);    --
            end,
        }
    },
    {
        name = "Finding types in argument list.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "ArgList_types", "3.x");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                ~import std::string;
                void go(string blah){}
            ]]);
            this.funcNode = this.root.Children[3];
            this.func = this.funcNode.Element;
            this.fo1 = this.funcNode.Children[1].Element;
        end,
        tests = {
            ["Function 'go' is added."] = function(this)
                Test.assertEquals(3, #this.root.Children);
                Test.assertEquals("go", this.root.Children[3].Name);
            end,
            ["Go's type is function."] = function(this)
                Test.assertEquals("Function", this.func.TypeName);
            end,
            ["Go has one child."] = function(this)
                Test.assertEquals(1, #(this.funcNode.Children));
            end,
            ["Go.Overload#1 is type 'FunctionOverload'"] = function(this)
				Test.assertEquals("FunctionOverload", this.fo1.TypeName);
            end,
            ["Go.Overload#1's return type is void."] = function(this)
                Test.assertEquals("void", this.fo1.ReturnType.Node.Name);
            end,
            ["Go.Overload#1's argument list has one entry."] = function(this)
                Test.assertEquals(1, #this.fo1.Arguments);
            end,
            ["Go.Overload#1's argument #1 is std::string."] = function(this)
                local args = this.fo1.Arguments;
                local number = #args;
                local arg1 = args[1];
                local arg1Type = arg1.Element.Type;
                local arg1TypeName = arg1Type.Node.FullName;
                Test.assertEquals("std::string", arg1TypeName);
            end,
        }
    },
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
