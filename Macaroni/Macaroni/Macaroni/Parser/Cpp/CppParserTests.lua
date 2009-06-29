require "Macaroni.Model.Context";
require "Macaroni.Parser.Cpp.CppParser";
require "Macaroni.Model.Source";

local Context = Macaroni.Model.Context;
local CppParser = Macaroni.Parser.Cpp.CppParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

Test.register(
{	
name = "CppParser tests",    
tests = {    
    ["Creating a CppParser in Lua."] = function(this)
        local parser = CppParser.Create();        
    end,
    
    {   
        name = "Reading a single namespace.",
        init = function(this)
            this.parser = CppParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1);
            local result = this.parser:Read(this.context, this.src, 
                [[
                    namespace Apple 
                    {
                    }
                ]]
                );            
        end,
        tests = {
            ["Only one node is found within the Context"] = function(this)
                Test.assert(1, #this.root.Children);
            end,
            ["Single node in Root is Apple."] = function(this)
                Test.assert("Apple", this.root.Children[0].Name);  
            end,
            ["Apple namespace has no children."] = function(this)
                Test.assert(0, #this.root.Children[0].Children);  
            end,
            ["Type of Node is Namespace."] = function(this)
                Test.assert("Namespace", this.root.Children[0].Member.TypeName);  
            end,
        }
    },
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
