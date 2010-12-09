require "Macaroni.Model.Context";
require "Macaroni.Environment.Messages";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";
require "Macaroni.Model.TypeList";

local Context = Macaroni.Model.Context;
local FileName = Macaroni.Model.FileName;
local Messages = Macaroni.Environment.Messages;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local Source = Macaroni.Model.Source;
local Type = Macaroni.Model.Type;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;
local TypeList = Macaroni.Model.TypeList;

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    self.stdstring = self.context.Root:FindOrCreate("std::string");
    self.stdstringType = Type.New(self.stdstring);
end

Test.register(
{	
    name = "TypeList Tests",    
    tests = {    
        {
            name = "Creating an empty TypeList.",
            init = function(self)               
                self.typeList = TypeList.New{};
            end,
            tests = {
                ["TypeList is empty."] = function(self)
                    Test.assert(0, #(self.typeList));
                end,                
            }
        },
        {
            name = "Creating a TypeList with one element.",
            init = function(self)  
                mixinContext(self);   
                self.typeList = TypeList.New{self.stdstringType};                
            end,
            tests = {
                ["TypeList has one element."] = function(self)
                    Test.assert(1, #(self.typeList));
                end,                
                ["TypeList first element is what we gave it."] = function(self)
                    Test.assert(self.stdstringType, self.typeList[1]);
                end,                
            }
        },        
    }    
}); -- End of register call
