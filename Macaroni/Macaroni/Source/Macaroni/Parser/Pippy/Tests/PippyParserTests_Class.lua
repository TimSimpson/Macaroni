require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.Cpp.Class";
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
require "Macaroni.Model.Cpp.Typedef";

local Access = Macaroni.Model.Cpp.Access;
local Class = Macaroni.Model.Cpp.Class;
local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;
local TypeList = Macaroni.Model.TypeList;
local Typedef = Macaroni.Model.Cpp.Typedef;

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
name = "PippyParser Tests :: Class",    
tests = {          
    {   
        name = "Creating an typedef for string.",
        init = function(self)
            self.parser = PippyParser.Create();     
            self.context = Context.New("{ROOT}");
            self.file = FileName.Create("Blah1.mcpp");           
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);
            
            self.parser:Read(self.context, self.src, [[                
                class Dog
                {
                    public int GetBarkCount(){ return bc; }
                    private int bc;
                }                
            ]]);         
            self.classNode = self.root.Children[2];   
            self.class = self.classNode.Member;   
        end,
        tests = {
            ["ClassNode has name 'Dog'."] = function(self)                               
                Test.assert("Dog", self.classNode.Name);                                  
            end,                        
            ["ClassNode has two children."] = function(self)                               
                Test.assert(2, #(self.classNode.Children));
            end,                        
            ["Class has private int."] = function(self)                               
                local bc = self.classNode.Children[2];
                Test.assert("bc", bc.Name);
                Test.assert(Access.Private, bc.Member.Access);                
            end,                        
            ["Class has public getter."] = function(self)                               
                local getter = self.classNode.Children[1];
                Test.assert("GetBarkCount", getter.Name);
                Test.assert(Access.Public, getter.Member.Access);                
            end,                        
        }
    },  
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
