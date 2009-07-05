require "Macaroni.Model.Context";
require "Macaroni.Parser.Cpp.CppParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";

local Context = Macaroni.Model.Context;
local CppParser = Macaroni.Parser.Cpp.CppParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;

Test.register(
{	
name = "CppParser Tests :: Namespaces",    
tests = {    
    ["Creating a CppParser in Lua."] = function(this)
        local parser = CppParser.Create();        
    end,
    
    ["Can handle an incorrect namespace."] = function(this)
        local parser = CppParser.Create();
        local context = Context.New("{ROOT}");
        local file = FileName.Create("Blah");           
        local root = context.Root;
        local src = Source.Create(file, 1);
        local status, err = pcall(function() 
            parser:Read(context, src, 
                [[  
                    namespace A{}
                    namEEespace Apple 
                    {
                    }
                    namespace E{}
                ]]
            );
        end);
        Test.assert(true, not status);
        Test.assert("Syntax or parser error.", err.Message);
        Test.assert("Blah", err.Source.FileName);
        Test.assert(2, err.Source.LineNumber);
    end,
    
    {   
        name = "Reading a single namespace.",
        init = function(this)
            this.parser = CppParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1);
            getmetatable(this.parser)["__tostring"]();
            local pee = tostring(this.parser);
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
        name = "Reading a single namespace twice.",
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
        name = "Reading two side-by-side namespaces.",
        init = function(this)
            this.parser = CppParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1);
            local result = this.parser:Read(this.context, this.src, 
                [[
                    namespace Orange
                    {
                    }
                    namespace Pear
                    {
                    }
                ]]
                );            
        end,
        tests = {
            ["Two nodes found within the Context root."] = function(this)
                Test.assert(2, #this.root.Children);
            end,
            ["Node names are Orange and Pear."] = function(this)
                Test.assert("Orange", this.root.Children[1].Name);  
                Test.assert("Pear", this.root.Children[2].Name);  
            end,
            ["Both nodes have no children."] = function(this)
                Test.assert(0, #this.root.Children[1].Children);  
                Test.assert(0, #this.root.Children[2].Children);  
            end,
            ["Nodes are both Namespaces."] = function(this)
                Test.assert("Namespace", this.root.Children[1].Member.TypeName);  
                Test.assert("Namespace", this.root.Children[2].Member.TypeName);  
            end,
        }
    },
    
     {   
        name = "Reading a nested namespace.",
        init = function(this)
            this.parser = CppParser.Create();
            this.context = Context.New("{ROOT}");
            this.file = FileName.Create("Blah");           
            this.root = this.context.Root;
            this.src = Source.Create(this.file, 1);
            local result = this.parser:Read(this.context, this.src, 
                [[
                    namespace Macaroni
                    {
                        namespace Model
                        {
                            namespace Cpp
                            {
                            }
                        }
                    }                    
                ]]
                );            
        end,
        tests = {
            ["Top node is Macaroni."] = function(this)
                local node = this.root;
                Test.assert(1, #node.Children);
                Test.assert("Macaroni", node.Children[1].Name);
                Test.assert("Namespace", node.Children[1].Member.TypeName);
            end,
            ["First nested entry is Model."] = function(this)
                local node = this.root.Children[1];
                Test.assert(1, #node.Children);
                Test.assert("Model", node.Children[1].Name);
                Test.assert("Namespace", node.Children[1].Member.TypeName);
            end,
            ["Second nested entry is Cpp."] = function(this)
                local node = this.root.Children[1].Children[1];
                Test.assert(1, #node.Children);
                Test.assert("Cpp", node.Children[1].Name);
                Test.assert("Namespace", node.Children[1].Member.TypeName);
            end,
        }
    },
   
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
