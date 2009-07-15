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

-- Convience method returns result of the parse, then status, and 
-- finally an err message.
local tryParse = function(text)
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
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.context, this.src, [[
                void main(){}
            ]]);         
            this.func = this.root.Children[2].Member;   
        end,
        tests = {
            ["Main is added."] = function(this)
                Test.assert(2, #this.root.Children);
                Test.assert("main", this.root.Children[2].Name);                                  
            end,            
            ["Main's type is function."] = function(this)                                
                Test.assert("Function", this.func.TypeName);                  
            end,            
            ["Main's return type is void."] = function(this)                                
                Test.assert("void", this.func.ReturnType.Node.Name);                  
            end,            
            ["Main's argument list is empty."] = function(this)                                
                Test.assert(0, #this.varfalse, this.func.Arguments);                  
            end,                        
        }
    },
      
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
