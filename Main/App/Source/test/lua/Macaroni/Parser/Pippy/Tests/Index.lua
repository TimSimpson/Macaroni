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
require "Macaroni/Parser/Pippy/Tests/GlobalKeyword";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Class";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Constructor";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Functions";
require "Macaroni/Parser/Pippy/Tests/Import";
require "Macaroni/Parser/Pippy/Tests/Milestones";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Namespace";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Operators";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Typedef";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Templates";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Variables";

local Context = require "Macaroni.Model.Context";
local Messages = require "Macaroni.Environment.Messages";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local FileName = require "Macaroni.Model.FileName";
local Source = require "Macaroni.Model.Source";

Test.register(
    {
        name = "PippyParser Tests :: Import",
        tests = {
            ["Running C++ parser method tests."] = function(this)
                PippyParser.RunTests();
            end
            }
    }
);
