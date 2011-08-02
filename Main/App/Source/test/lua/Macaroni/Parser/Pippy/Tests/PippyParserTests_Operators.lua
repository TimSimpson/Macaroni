--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use self file except in compliance with the License.
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
require "Macaroni.Model.Context";
require "Macaroni.Environment.Messages";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";

local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;
local TypeList = Macaroni.Model.TypeList;

require "Macaroni/Parser/Pippy/Tests/TryParse"

function operatorTest(actualCode, expectedNode)
	return {   
        name = expectedNode,
        init = function(self)
			ParserTest.init(self)
            self.parser:Read(self.library, self.src, [[
                class Blah
                {
					public int ]] .. actualCode .. [[(int a, int b) {}
                }
            ]]);                                      
        end,
        tests = {
            ["Operator is created as node 'operator new' (with a space)."] 
            = function(self)
                Test.assertEquals(2, #self.root.Children);
                Test.assertEquals("Blah", self.root.Children[2].FullName);                                  
                local Blah = self.root.Children[2];
                Test.assertEquals(1, #Blah.Children);
                Test.assertEquals("Blah::" .. expectedNode, 
                                  Blah.Children[1].FullName);
                local funcNode = Blah.Children[1];                
                Test.assertEquals("Function", funcNode.Member.TypeName);
                Test.assertEquals(1, #(funcNode.Children));
				local ol = funcNode.Children[1]
				Test.assertEquals("FunctionOverload", ol.Member.TypeName);
				Test.assertEquals("FunctionOverload", ol.Member.TypeName);
				Test.assertEquals(2, #ol.Member.Arguments);
            end,                   
        }
    }
end
        
Test.register({	
	name = "PippyParser Tests :: Operators",    
	tests = {        
		operatorTest("operator   +  ", "operator+"),
		operatorTest("operator-", "operator-"),
		operatorTest("operator !=", "operator!="),
		operatorTest("operator  new ", "operator new"),
		operatorTest("operator delete ", "operator delete"),
	} -- end of tests table 
}); -- End of register call
