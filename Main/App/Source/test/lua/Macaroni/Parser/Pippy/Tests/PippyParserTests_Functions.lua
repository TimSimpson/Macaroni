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
            this.library = this.context:FindOrCreateLibrary("Tests", "PippyParserTests_Functions", "9.5");
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.library, this.src, [[
                void main(){}
            ]]);         
            this.funcNode = this.root.Children[2];
            this.func = this.funcNode.Member;               
            this.fo1 = this.funcNode.Children[1].Member;
        end,
        tests = {
            ["Main is added."] = function(this)
                Test.assert(2, #this.root.Children);
                Test.assert("main", this.root.Children[2].Name);                                  
            end,            
            ["Main's type is function."] = function(this)                                
                Test.assert("Function", this.func.TypeName);                  
            end,     
            ["Main has one overload."] = function(this)
				Test.assert(1, #(this.funcNode.Children));
            end,       
            ["Main.Overload#0 type is overload."] = function(this)
				Test.assert("FunctionOverload", this.fo1.TypeName);
            end,       
            ["Main.Overload#0 return type is void."] = function(this)                                
                Test.assert("void", this.fo1.ReturnType.Node.Name);  --                
            end,            
            ["Main.Overload#0 argument list is empty."] = function(this)                                
                Test.assert(0, #this.fo1.Arguments);    --              
            end,                        
        }
    },
    {   
        name = "Finding types in argument list.",
        init = function(this)
            this.parser = PippyParser.Create();     
            this.context = Context.New("{ROOT}");
            this.library = this.context:FindOrCreateLibrary("Tests", "ArgList_types", "3.x");
            this.file = FileName.Create("Blah1.mcpp");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1, 1);
            
            this.parser:Read(this.library, this.src, [[
                ~import std::string;
                void go(string blah){}
            ]]);         
            this.funcNode = this.root.Children[3];
            this.func = this.funcNode.Member;   
            this.fo1 = this.funcNode.Children[1].Member;
        end,
        tests = {
            ["Function 'go' is added."] = function(this)
                Test.assert(3, #this.root.Children);
                Test.assert("go", this.root.Children[3].Name);                                  
            end,            
            ["Go's type is function."] = function(this)                                
                Test.assert("Function", this.func.TypeName);                  
            end,            
            ["Go has one child."] = function(this)                                
                Test.assert(1, #(this.funcNode.Children));                  
            end,            
            ["Go.Overload#1 is type 'FunctionOverload'"] = function(this)
				Test.assert("FunctionOverload", this.fo1.TypeName);
            end,
            ["Go.Overload#1's return type is void."] = function(this)                                
                Test.assert("void", this.fo1.ReturnType.Node.Name);
            end,            
            ["Go.Overload#1's argument list has one entry."] = function(this)                 
                Test.assert(1, #this.fo1.Arguments);    
            end,   
            ["Go.Overload#1's argument #1 is std::string."] = function(this)
                local args = this.fo1.Arguments;
                local number = #args;
                local arg1 = args[1];                   
                local arg1Type = arg1.Member.Type;
                local arg1TypeName = arg1Type.Node.FullName;       
                Test.assert("std::string", arg1TypeName); 
            end,                       
        }
    },  
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
