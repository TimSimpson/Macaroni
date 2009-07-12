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
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.context, this.src, [[
                import std::string;
                
                string hello;
                const string & me;
                
                namespace Apple
                {
                    double donuts;
                }
            ]]);         
            this.var = this.root.Children[2].Member;   
        end,
        tests = {
            ["Hello."] = function(this)
                Test.assert(5, #this.root.Children);
                local hello = this.root.Children[3];                
                Test.assert("hello", hello.Name);
                Test.assert("Variable", hello.Member.TypeName);
                Test.assert("std::string", hello.Member.TypeNode.FullName);
            end,            
            ["donuts"] = function(this)                
                local donuts = this.root.Children[5].Children[1];
                Test.assert("Apple::donuts", donuts.FullName);
                Test.assert("Variable", donuts.Member.TypeName);
                Test.assert("double", donuts.Member.TypeNode.Name);
            end,            
        }
    },
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
