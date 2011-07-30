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
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";
--require "Macaroni.Model.Cpp.Typedef";

local Context = Macaroni.Model.Context;
local Messages = Macaroni.Environment.Messages;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local FileName = Macaroni.Model.FileName;
local Source = Macaroni.Model.Source;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;
local TypeList = Macaroni.Model.TypeList;
--local Typedef = Macaroni.Model.Cpp.Typedef;

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
name = "PippyParser Tests :: Typedefs",    
tests = {          
    {   
        name = "Creating an typedef for string.",
        init = function(self)
            self.parser = PippyParser.Create();     
            self.context = Context.New("{ROOT}");
            self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            self.file = FileName.Create("Blah1.mcpp");           
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);
            
            self.parser:Read(self.library, self.src, [[
                ~import std::string;
                
                typedef std::string stringType;
            ]]);         
            self.stringType = self.root.Children[3].Member;   
        end,
        tests = {
            ["Typedef has name 'stringType'."] = function(self)                
                Test.assertEquals(3, #self.root.Children);
                Test.assertEquals("stringType", self.root.Children[3].Name);                                  
            end,            
            ["stringType's type is typedef."] = function(self)                                
                Test.assertEquals("Typedef", self.stringType.TypeName);                  
            end,            
            ["The typedef's type is a node."] = function(self)                
                local type = self.stringType.Type;
                Test.assertEquals("std::string", type.Node.FullName);
            end,            
            ["The typedef's type has empty arg list."] = function(self)                                
                local type = self.stringType.Type;
                local typeArgs = type.TypeArguments;
                Test.assertEquals(0, #(typeArgs));            
            end,                        
        }
    },
    {   
        name = "Creating an typedef for vector of strings.",
        init = function(self)
            self.parser = PippyParser.Create();     
            self.context = Context.New("{ROOT}");
            self.library = self.context:FindOrCreateLibrary("Tests", "Test", "1.0");
            self.file = FileName.Create("Blah1.mcpp");           
            self.root = self.context.Root;
            self.src = Source.Create(self.file, 1, 1);
            
            self.parser:Read(self.library, self.src, [[
                ~import std::string;
                ~import std::vector;
                
                typedef std::vector<std::string> stringArray;
            ]]);         
            self.typedefNode = self.root.Children[3];             
            self.typedef = self.typedefNode.Member;            
        end,
        tests = {
            ["Typedef has name 'stringArray'."] = function(self)
                Test.assertEquals("stringArray", self.typedefNode.Name);                                  
            end,            
            ["The Member's type is typedef."] = function(self)                                
                Test.assertEquals("Typedef", self.typedef.TypeName);                  
            end,            
            ["The typedef's type is the vector node."] = function(self)                
                local type = self.typedef.Type;
                Test.assertEquals("std::vector", type.Node.FullName);
            end,            
            ["The typedef's type has an argument list of the String type."] = function(self)                                
                local type = self.typedef.Type;
                local typeArgs = type.TypeArguments;
                Test.assertEquals(1, #(typeArgs));

                local typeArg1 = typeArgs[1];            
                Test.assertEquals("std::vector", typeArg1.Node.FullName);            
                Test.assertEquals(1, #(typeArg1.Arguments));
                Test.assertEquals("std::string", typeArg1.Arguments[1].Node.FullName);            
            end,                        
        }
    },  
} -- end of tests table ( I skipped some indentation way above here).
}); -- End of register call
