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
require "Macaroni.Model.Library";
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
    self.library = self.context:FindOrCreateLibrary("Tests", "TypeArgumentListTests", "1.0");
    
    local parser = PippyParser.Create();         
    local file = FileName.Create("Blah1.mcpp");           
    local root = self.context.Root;
    local src = Source.Create(file, 1, 1);
            
    parser:Read(self.library, src, [[ 
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
    self.vectorType = Type.New(self.vector);
    self.event = self.context.Root:Find("Event");
    self.eventType = Type.New(self.event);
    self.message = self.context.Root:Find("Event::Message");
    self.messageType = Type.New(self.message);
    self.eventId  = self.context.Root:Find("EventId");
    self.eventId = Type.New(self.eventId);
    self.audioData  = self.context.Root:Find("AudioData");
    self.audioDataType = Type.New(self.audioData);
    self.gfxData  = self.context.Root:Find("GfxData");
    self.gfxDataType = Type.New(self.gfxData);
    self.stdstring = self.context.Root:Find("std::string");
    self.stdstringType = Type.New(self.stdstring);
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
                    Test.assertEquals(0, #(self.typeArgList));
                end,                
            }
        },
        {
            name = "Creating a TypeArgumentList with one element.",
            init = function(self)                
                mixinContext(self);
                local args = TypeList.New{self.stdstringType};
                self.typeArg_vectorUsesString = TypeArgument.New(self.vector, args);
                self.typeArgList = TypeArgumentList.New{self.typeArg_vectorUsesString};
            end,
            tests = {
                ["TypeArgumentList has one element."] = function(self)
                    Test.assertEquals(1, #(self.typeArgList));
                end,
                ["TypeArgumentList's element is the thing we created for it."] = function(self)
                    Test.assertEquals(self.typeArg_vectorUsesString, self.typeArgList[1]);       
                end,
            }
        },
        {
            name = "Creating a TypeArgumentList with multiple elements.",
            init = function(self)
                mixinContext(self);                     
                local args = TypeList.New{self.eventIdType};
                self.typeArg_eventUsesEventId = TypeArgument.New(self.event, args);
                local args2 = TypeList.New{self.stdstringType, self.gfxDataType};
                self.typeArg_messageUsesStringAndGfx = TypeArgument.New(self.message, args2);
                
                self.typeArgList = TypeArgumentList.New{
                    self.typeArg_eventUsesEventId, 
                    self.typeArg_messageUsesStringAndGfx
                };
            end,
            tests = {
                ["TypeArgumentList has two elements."] = function(self)
                    Test.assertEquals(2, #(self.typeArgList));           
                end,
                ["Arguments #1 is that Event takes EventId."] = function(self)
                    local expected = self.typeArg_eventUsesEventId;
                    local actual = self.typeArgList[1];
                    Test.assertEquals(expected, actual);
                end,
                ["Arguments #2 is that Message accepts a String, and GfxData."] = function(self)
                    local expected = self.typeArg_messageUsesStringAndGfx;
                    local actual = self.typeArgList[2];
                    Test.assertEquals(expected, actual);
                end,
            }
        }
    }    
}); -- End of register call
