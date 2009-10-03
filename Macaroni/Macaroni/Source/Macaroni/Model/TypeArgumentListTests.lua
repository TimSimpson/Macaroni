require "Macaroni.Model.Context";
require "Macaroni.Environment.Messages";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";
require "Macaroni.Model.Source";
require "Macaroni.Model.TypeArgument";
require "Macaroni.Model.TypeArgumentList";

local Context = Macaroni.Model.Context;
local FileName = Macaroni.Model.FileName;
local Messages = Macaroni.Environment.Messages;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local PippyParser = Macaroni.Parser.Pippy.PippyParser;
local Source = Macaroni.Model.Source;
local TypeArgument = Macaroni.Model.TypeArgument;
local TypeArgumentList = Macaroni.Model.TypeArgumentList;

--[[
Its easy to see how a vector class could accept a type argument.

But also imagine that the "Event" class could accept one, and its nested class
"Message" could accept two.
i.e.,
typedef Event<EventId>::Message<std::string, GfxData> GfxEventMessage;

The parser currently can't add template tags to these classes, but that
doesn't matter since the TypeArguments will work regardless (the premise is if
such code is generated the authentic C++ compiler will fail it later- 
Macaroni can't check for these without becoming more complicated and requiring
type information earlier.
]]--

local function mixinContext(self)
    self.context = Context.New("{ROOT}");
    
    local parser = PippyParser.Create();         
    local file = FileName.Create("Blah1.mcpp");           
    local root = self.context.Root;
    local src = Source.Create(file, 1, 1);
            
    parser:Read(self.context, src, [[ 
        namespace std
        {
            class string{}            
        }
        class Vector {}; 
        class Event
        {
            class Message
            {
            }
        };     
        class EventId{};
        class AudioData{};
        class GfxData{};
    ]]);         
    
    self.vector = self.context.Root:Find("Vector");
    self.event = self.context.Root:Find("Event");
    self.message = self.context.Root:Find("Event::Message");
    self.eventId  = self.context.Root:Find("EventId");
    self.audioData  = self.context.Root:Find("AudioData");
    self.gfxData  = self.context.Root:Find("GfxData");
    self.stdstring = self.context.Root:Find("std::string");
    
end

Test.register(
{	
    name = "TypeArgumentList Tests",    
    tests = {    
        {
            name = "Creating a TypeArgumentList which is empty.",
            init = function(self)               
                self.typeArgList = TypeArgumentList.New{};
            end,
            tests = {
                ["TypeArgumentList has no elements."] = function(self)
                    Test.assert(0, #(self.typeArgList));
                end,                
            }
        },
        {
            name = "Creating a TypeArgumentList with one element.",
            init = function(self)                
                mixinContext(self);
                local args = NodeList.New{self.stdstring};
                self.typeArg_vectorUsesString = TypeArgument.New(self.vector, args);
                self.typeArgList = TypeArgumentList.New{self.typeArg_vectorUsesString};
            end,
            tests = {
                ["TypeArgumentList has one element."] = function(self)
                    Test.assert(1, #(self.typeArgList));
                end,
                ["TypeArgumentList's element is the thing we created for it."] = function(self)
                    Test.assert(self.typeArg_vectorUsesString, self.typeArgList[1]);       
                end,
            }
        },
        {
            name = "Creating a TypeArgumentList with multiple elements.",
            init = function(self)
                mixinContext(self);                     
                local args = NodeList.New{self.eventId};
                self.typeArg_eventUsesEventId = TypeArgument.New(self.event, args);
                local args2 = NodeList.New{self.stdstring, self.gfxData};
                self.typeArg_messageUsesStringAndGfx = TypeArgument.New(self.message, args2);
                
                self.typeArgList = TypeArgumentList.New{
                    self.typeArg_eventUsesEventId, 
                    self.typeArg_messageUsesStringAndGfx
                };
            end,
            tests = {
                ["TypeArgumentList has two elements."] = function(self)
                    Test.assert(2, #(self.typeArgList));           
                end,
                ["Arguments #1 is that Event takes EventId."] = function(self)
                    local expected = self.typeArg_eventUsesEventId;
                    local actual = self.typeArgList[1];
                    Test.assert(expected, actual);
                end,
                ["Arguments #2 is that Message accepts a String, and GfxData."] = function(self)
                    local expected = self.typeArg_messageUsesStringAndGfx;
                    local actual = self.typeArgList[2];
                    Test.assert(expected, actual);
                end,
            }
        }
    }    
}); -- End of register call
