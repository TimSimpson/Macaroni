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
require "Macaroni.Model.Context";
require "Macaroni.Environment.Messages";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";

local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

        
Test.register(
{	
name = "PippyParser Tests :: Milestones",    
tests = {          
    {   
        name = "Milestone 1- Global vars in Namespace",
        init = function(this)
            this.parser = PippyParser.Create();     
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.library, this.src, [[
                ~import std::string;
                
                string hello;
                string & me;
                
                namespace Apple
                {
                    double donuts;
                }
            ]]);         
            this.var = this.root.Children[2].Member;   
        end,
        tests = {
            ["Hello."] = function(this)
                Test.assertEquals(5, #this.root.Children);
                local hello = this.root.Children[3];                
                Test.assertEquals("hello", hello.Name);
                Test.assertEquals("Variable", hello.Member.TypeName);
                Test.assertEquals("std::string", hello.Member.Type.Node.FullName);
            end,            
            ["donuts"] = function(this)                
                local donuts = this.root.Children[5].Children[1];
                Test.assertEquals("Apple::donuts", donuts.FullName);
                Test.assertEquals("Variable", donuts.Member.TypeName);
                Test.assertEquals("double", donuts.Member.Type.Node.Name);
            end,            
        }
    },
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
