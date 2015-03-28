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

local Context = require "Macaroni.Model.Context";
local FileName = require "Macaroni.Model.FileName";
local Messages = require "Macaroni.Environment.Messages";
local Node = require "Macaroni.Model.Node";
local NodeList = require "Macaroni.Model.NodeList";
local Path = require "Macaroni.IO.Path";
local PippyParser = require "Macaroni.Parser.Pippy.PippyParser";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local TypeArgumentList = require "Macaroni.Model.TypeArgumentList";
local TypeList = require "Macaroni.Model.TypeList";

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
    self.target = self.context:Group("Tests"):Project("TypeArgumentListTests"):Version("1.0"):Target("hi");

    local parser = PippyParser.Create();
    local p = Path.New("Blah1.mcpp")
    local file = FileName.Create(p);
    local root = self.context.Root;
    local src = Source.Create(file, 1, 1);

    parser:Read(self.target, src, [[
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
    self.vectorType = self.context:CreateType()
    self.vectorType.Node = self.vector;
    self.event = self.context.Root:Find("Event");
    self.eventType = self.context:CreateType()
    self.eventType.Node = self.event;
    self.message = self.context.Root:Find("Event::Message");
    Test.assertEquals()
    Test.assertEquals("Event::Message", tostring(self.message))

    self.messageType = self.context:CreateType()
    self.messageType.Node = self.message;
    self.eventIdNode  = self.context.Root:Find("EventId");
    self.eventId = self.context:CreateType()
    self.eventId.Node = self.eventIdNode;
    self.audioData  = self.context.Root:Find("AudioData");
    self.audioDataType  = self.context:CreateType()
    self.audioDataType.Node = self.audioData;
    self.gfxData  = self.context.Root:Find("GfxData");
    self.gfxDataType = self.context:CreateType()
    self.gfxDataType.Node = self.gfxData;
    self.stdstring = self.context.Root:Find("std::string");
    self.stdstringType = self.context:CreateType()
    self.stdstringType.Node = self.stdstring;
end

Test.register(
{
    name = "TypeArgumentList Tests",
    tests = {
        {
            name = "Creating a TypeArgumentList which is empty.",
            init = function(self)
                mixinContext(self);
                self.type = self.context:CreateType();
                self.typeArgList = self.type:AddArgument(0);
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
                self.rootType = self.context:CreateType();
                self.typeArgList = self.rootType:AddArgument(0);

                self.typeArg_vectorUsesString = self.typeArgList:CreateType();
                self.typeArg_vectorUsesString.Node = self.vector
                --self.typeArg_vectorUsesString:AddArgument
            end,
            tests = {
                ["TypeArgumentList has one element."] = function(self)
                    Test.assertEquals(1, #(self.typeArgList));
                end,

            }
        },
    }
}); -- End of register call
