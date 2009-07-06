require "Macaroni.Model.Context";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";

local Context = Macaroni.Model.Context;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

Test.register(
{	
name = "PippyParser Tests :: Simple",    
tests = {        
    {   
        name = "Reading a single namespace.",
        init = function(this)
            this.parser = PippyParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1);
            getmetatable(this.parser)["__tostring"]();
            local pee = tostring(this.parser);
            local status, err = pcall(function()
                local result = this.parser:Read(this.context, this.src, 
                    [[                    
                        Apple :: namespace
                        {
                        }
                    ]]
                    );            
            end);
        end,
        tests = {
            --[[["Only one node is found within the Context"] = function(this)
                Test.assert(1, #this.root.Children);
            end,
            ["Single node in Root is Apple."] = function(this)
                Test.assert("Apple", this.root.Children[1].Name);  
            end,
            ["Apple namespace has no children."] = function(this)
                Test.assert(0, #this.root.Children[1].Children);  
            end,
            ["Type of Node is Namespace."] = function(this)
                Test.assert("Namespace", this.root.Children[1].Member.TypeName);  
            end,]]--
        }
    },    
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
