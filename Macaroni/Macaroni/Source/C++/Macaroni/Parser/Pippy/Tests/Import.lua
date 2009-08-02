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
name = "PippyParser Tests :: Import",    
tests = {            
     {   
        name = "Importing stuff adds it.",
        init = function(this)
            this.parser = PippyParser.Create();     
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.context, this.src, [[
                import System::Graphics::BlahBlahBlah;
                import Turtles::Fight::With::Honor;
                import Cat;
            ]]);            
        end,
        tests = {
            ["The imports are added as maleable nodes."] = function(this)
                Test.assert(4, #this.root.Children);
                Test.assert("{C++ Primitives}", this.root.Children[1].Name);  
                Test.assert("System", this.root.Children[2].Name);  
                Test.assert("Turtles", this.root.Children[3].Name);  
                Test.assert("Cat", this.root.Children[4].Name);  
            end,
            ["Sub nodes are also added."] = function(this)
                Test.assert("Turtles::Fight::With::Honor", 
                            this.root.Children[3].Children[1].Children[1].FullName);  
            end,           
        }
    },
    
  
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
