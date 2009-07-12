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
    ["Running C++ parser method tests."] = function(this)
        PippyParser.RunTests();
    end,
    
    
    {   
        name = "Reading a single namespace.",
        init = function(this)
            this.parser = PippyParser.Create();     
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.context, this.src, [[
                namespace Apple { }
            ]]);
            
        end,
        tests = {
            ["Only one node is found within the Context"] = function(this)
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
            end,
        }
    },    
    
    {   
        name = "Namespace parsing throws errors on invalid conditions.",
         
        -- Convience method returns result of the parse, then status, and 
        -- finally an err message.
        tryParse = function(text)
            local parser = PippyParser.Create();
            local context = Context.New("{ROOT}");
            local file = FileName.Create("Blah1.mcpp");
            local src = Source.Create(file, 1, 1);
            
            local result = nil;
            local status, err = pcall(function() 
                    result = parser:Read(context, src, text);
                end
            );
            return status, err;
        end,
        
        init = function(this)           
        end,
        
        tests = {
            ["Garbage appears before Namespace."] = function(this)
                local status, err = this.tryParse("dsjfk");
                Test.assert(false, status);
                Test.assert(1, err.Source.Line);                
                Test.assert("Syntax error.", err.Message);                
            end,
            
        }
    },
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
