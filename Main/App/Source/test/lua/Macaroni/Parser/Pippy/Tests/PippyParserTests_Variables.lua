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

local Context = require "Macaroni.Model.Context";
local Messages = require "Macaroni.Environment.Messages";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Path = require "Macaroni.IO.Path";
local Source = require "Macaroni.Model.Source";

-- Convience method returns result of the parse, then status, and
-- finally an err message.
local tryParse = function(text)
    local parser = PippyParser.Create();
    local context = Context.New("{ROOT}");
    local library = context:FindOrCreateLibrary("Tests", "tryParse", "1.0");
    local file = FileName.Create(Path.New("", "Blah1.mcpp"));
    local src = Source.Create(file, 1, 1);

    local result = nil;
    local status, err = pcall(function()
            result = parser:Read(library, src, text);
        end
    );
    return status, err;
end


Test.register(
{
name = "PippyParser Tests :: Variables",
tests = {
    {
        name = "Defining some primitive vars.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "PippyParserTests_VARS", "44");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is not const."] = function(this)
                Test.assertEquals(false, this.var.Type.Const);
            end,
        }
    },

    {
        name = "Defining two primitive vars.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fgd", "4");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char c;
                double k;
            ]]);
            this.var = this.root.Children[2].Element;
            this.var2 = this.root.Children[3].Element;
        end,
        tests = {
            ["C and K are siblings."] = function(this)
                Test.assertEquals(3, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
                Test.assertEquals("k", this.root.Children[3].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is not const."] = function(this)
                Test.assertEquals(false, this.var.Type.Const);
            end,
            ["K is not const."] = function(this)
                Test.assertEquals(false, this.var2.Type.Const);
            end,
        }
    },



    {
        name = "Defining some a primitive with const.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                const char c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C is not const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
        }
    },

    {
        name = "Defining some a primitive with const in position two.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char const c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C is const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
            ["C is not a pointer."] = function(this)
                Test.assertEquals(false, this.var.Type.Pointer);
            end,
            ["C is not a reference."] = function(this)
                Test.assertEquals(false, this.var.Type.Reference);
            end,
        }
    },

    {
        name = "Defining var with complexName may create additional nodes.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char const Donut::K;
            ]]);
        end,
        tests = {
            ["Donut, an unknown node, is created to nest K."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("Donut", this.root.Children[2].Name);
                Test.assertEquals(nil, this.root.Children[2].Element);
            end,
            ["K is found within Donut."] = function(this)
                Test.assertEquals(1, #this.root.Children[2].Children);
                Test.assertEquals("Donut::K", this.root.Children[2].Children[1].FullName);
                Test.assertEquals("Variable", this.root.Children[2].Children[1].Element.TypeName);
            end,
        }
    },

    {
        name = "Double definition is OK if it matches.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "gdfd", "423");
            this.file = FileName.Create(Path.New("", "DoubleDribbler.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char c;  // Apparently, this was allowed but isn't anymore.
                double const k; // I'd like to keep the feature though so IOU
                const double k; // one bug fix. ~ Tim of the Past
            ]]);
            this.var = this.root.Children[2].Element;
            this.var2 = this.root.Children[3].Element;
        end,
        tests = {
            ["C and K are siblings."] = function(this)
                Test.assertEquals(3, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
                Test.assertEquals("k", this.root.Children[3].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is not const."] = function(this)
                Test.assertEquals(false, this.var.Type.Const);
            end,
            ["K is const."] = function(this)
                Test.assertEquals(true, this.var2.Type.Const);
            end,
        }
    },

     {
        name = "Defining a pointer.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "rfdg", "x");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                const char * c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
            ["C is a pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.Pointer);
            end,
        }
    },

    {
        name = "Defining a pointer, 2.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char const * c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
            ["C is a pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.Pointer);
            end,
        }
    },

    {
        name = "Defining a const pointer to const.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char const * const c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
            ["C is a pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.Pointer);
            end,
            ["C is a const pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.ConstPointer);
            end,
            ["C is not a reference."] = function(this)
                Test.assertEquals(false, this.var.Type.Reference);
            end,
        }
    },

    {
        name = "Defining a reference.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char & c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is not const."] = function(this)
                Test.assertEquals(false, this.var.Type.Const);
            end,
            ["C is not a pointer."] = function(this)
                Test.assertEquals(false, this.var.Type.Pointer);
            end,
            ["C is not a const pointer."] = function(this)
                Test.assertEquals(false, this.var.Type.ConstPointer);
            end,
            ["C is a reference."] = function(this)
                Test.assertEquals(true, this.var.Type.Reference);
            end,
        }
    },

    {
        name = "Defining a reference to const pointer to const.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "fsds", "c");
            this.file = FileName.Create(Path.New("", "Blah1.mcpp"));
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);

            this.parser:Read(this.library, this.src, [[
                char const * const & c;
            ]]);
            this.var = this.root.Children[2].Element;
        end,
        tests = {
            ["C, the var, is added."] = function(this)
                Test.assertEquals(2, #this.root.Children);
                Test.assertEquals("c", this.root.Children[2].Name);
            end,
            ["C's member is a Variable."] = function(this)
                Test.assertEquals("Variable", this.var.TypeName);
            end,
            ["C is const."] = function(this)
                Test.assertEquals(true, this.var.Type.Const);
            end,
            ["C is a pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.Pointer);
            end,
            ["C is a const pointer."] = function(this)
                Test.assertEquals(true, this.var.Type.ConstPointer);
            end,
            ["C is a reference."] = function(this)
                Test.assertEquals(true, this.var.Type.Reference);
            end,
        }
    },

--TODO: Figure out why the heck ParserException quit getting raised correctly.
--[[
    ["Const without ID is invalid."] = function(this)
        local status, err = this.tryParse("const []");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(7, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Const.MaybeBeforeVar"), err.Message);
    end,

    ["Const without ID is invalid."] = function(this)
        -- I tried this in C++ and its illegal.
        local status, err = this.tryParse("const & char c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(7, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Const.MaybeBeforeVar"), err.Message);
    end,

    ["Const without ID is invalid."] = function(this)
        -- I tried this in C++ and its illegal.
        local status, err = tryParse("const * char c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(7, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.ConstMaybeBeforeVar"), err.Message);
    end,

    ["Unknown typename is invalid."] = function(this)
        local status, err = tryParse("const quote c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(7, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.UnknownTypeName"), err.Message);
    end,

    ["Unknown typename is invalid, method 2."] = function(this)
        local status, err = tryParse("quote c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(1, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.UnknownTypeName"), err.Message);
    end,


    ["Const appearing twice is invalid."] = function(this)
        local status, err = tryParse("const char const c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(12, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.ConstSeenTwice"), err.Message);
    end,

    ["Name must follow type and const."] = function(this)
        local status, err = tryParse("const char - c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(12, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.NameExpected"), err.Message);
    end,

    ["Name must follow type and const, 2."] = function(this)
        local status, err = tryParse("char - c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(6, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.NameExpected"), err.Message);
    end,

    ["Name must follow type and const, 3."] = function(this)
        local status, err = tryParse("char const - c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(12, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.NameExpected"), err.Message);
    end,

    ["Double definition if conflicting is invalid."] = function(this)
        local status, err = tryParse("char c; const char c;");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(9, err.Source.Column);
        --Test.assertEquals(Messages.Get("CppParser.Variable.NameExpected"), err.Message);
    end,

    ["Must see ; at the end of free standing var."] = function(this)
        local status, err = tryParse("char c");
        Test.assertEquals(false, status);
        Test.assertEquals(1, err.Source.Line);
        Test.assertEquals(7, err.Source.Column);
        Test.assertEquals(Messages.Get("CppParser.Variable.SemicolonExpected"), err.Message);
    end,
]]--

} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
