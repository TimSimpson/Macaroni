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
local TypeModifiers = require "Macaroni.Model.TypeModifiers";
local SimpleTypeModifiers = require "Macaroni.Model.SimpleTypeModifiers";

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
    self.target = self.context:Group("Tests"):Project("TypeTests"):Version("1.5"):Target("hi");

    local parser = PippyParser.Create();
    local file = FileName.Create(Path.New("", "Blah1.mcpp"));
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
    self.event = self.context.Root:Find("Event");
    self.message = self.context.Root:Find("Event::Message");
    self.eventId  = self.context.Root:Find("EventId");
    self.audioData  = self.context.Root:Find("AudioData");
    self.gfxData  = self.context.Root:Find("GfxData");
    self.stdstring = self.context.Root:Find("std::string");
end

Test.register(
{
    name = "Type Tests",
    tests = {
        {
            name = "Creating a Type which represents std::string.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring;
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
            }
        },
        {
            name = "Creating a Type which represents const std::string.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring;
                local mods = self.typeString.Modifiers
                mods.Const = true
                self.typeString.Modifiers = mods
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
                ["Type is const."] = function(self)
                    Test.assertEquals(true, self.typeString.Modifiers.Const);
                end,
            }
        },
        {
            name = "Creating a Type which represents std::string *.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring;
                pointer = SimpleTypeModifiers.New()
                mods = self.typeString.Modifiers
                Test.assertEquals(true, pointer ~= nil)
                Test.assertEquals(true, mods.Pointer == nil)
                mods:SetPointer(pointer)
                Test.assertEquals(true, mods.Pointer ~= nil)
                self.typeString.Modifiers = mods
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
                ["Type is pointer."] = function(self)
                    Test.assertEquals(true,
                                      self.typeString.Modifiers.Pointer ~= nil);
                end,
            }
        },
        {
            name = "Creating a Type which represents std::string &.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring
                local mods = self.typeString.Modifiers
                mods.Reference = true
                self.typeString.Modifiers = mods
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
                ["Type is reference."] = function(self)
                    Test.assertEquals(true, self.typeString.Modifiers.Reference);
                end,
            }
        },
        {
            name = "Creating a Type which represents std::string * const.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring
                local pointer = SimpleTypeModifiers.New()
                pointer.Const = true
                local mods = self.typeString.Modifiers
                mods:SetPointer(pointer)
                self.typeString.Modifiers = mods
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
                ["Type is const pointer."] = function(self)
                    Test.assertEquals(true, self.typeString.Modifiers.ConstPointer);
                end,
            }
        },
        {
            name = "Creating a Type which represents std::string * const.",
            init = function(self)
                mixinContext(self);
                self.typeString = self.context:CreateType();
                self.typeString.Node = self.stdstring
                local pointer = SimpleTypeModifiers.New()
                pointer.Const = true
                local mods = self.typeString.Modifiers
                mods:SetPointer(pointer)
                mods.Const = true
                self.typeString.Modifiers = mods
            end,
            tests = {
                ["Type is const."] = function(self)
                    Test.assertEquals(true, self.typeString.Modifiers.Const);
                end,
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assertEquals(self.stdstring, self.typeString.Node);
                end,
                ["Type is const pointer."] = function(self)
                    Test.assertEquals(
                        true, self.typeString.Modifiers.ConstPointer);
                end,
                ["Type is pointer."] = function(self)
                    Test.assertEquals(
                        true, self.typeString.Modifiers.Pointer ~= nil);
                end,
            }
        },
        {
            name = "Creating a Type representing Vector<std::string>.",
            init = function(self)
                mixinContext(self);
                self.type = self.context:CreateType()
                self.type.Node = self.vector
                self.typeArgs = self.type:AddArgument(0)
                self.typeArg1 = self.typeArgs:CreateType()
                self.typeArg1.Node = self.stdstring

                -- self.typeArg1 = TypeArgument.New(
                --         self.vector,
                --         NodeList.New{ self.stdstring }
                --     );
                -- self.typeArgs = TypeArgumentList.New{self.typeArg1};
                -- self.type = Type.New(self.vector, {}, self.typeArgs);
            end,
            tests = {
                ["Type points to Node representing Vector."] = function(self)
                    Test.assertEquals(self.vector, self.type.Node);
                end,
            }
        },
    }
}); -- End of register call
