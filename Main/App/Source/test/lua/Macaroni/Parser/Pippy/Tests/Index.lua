require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Class";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Functions";
require "Macaroni/Parser/Pippy/Tests/Import";
require "Macaroni/Parser/Pippy/Tests/Milestones";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Namespace";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Typedef";
require "Macaroni/Parser/Pippy/Tests/PippyParserTests_Variables";

local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

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