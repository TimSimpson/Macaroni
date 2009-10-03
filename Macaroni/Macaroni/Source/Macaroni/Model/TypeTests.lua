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
    name = "Type Tests",    
    tests = {    
        {
            name = "Creating a Type which represents std::string.",
            init = function(self) 
                mixinContext(self);               
                self.typeString = Type.New(self.stdstring);
            end,
            tests = {
                ["Type points to the Node represent std::string."] = function(self)
                    Test.assert(self.stdstring, self.typeString.Node);
                end,                
            }
        },
        {
            name = "Creating a Type representing Vector<std::string>.",
            init = function(self)  
                mixinContext(self);   
                self.typeArg1 = TypeArgument.New(
                        self.vector,
                        NodeList.New{ self.stdstring } 
                    );                
                self.typeArgs = TypeArgumentList.New{self.typeArg1};
                self.type = Type.New(self.vector, self.typeArgs);                
            end,
            tests = {
                ["Type points to Node representing Vector."] = function(self)
                    Test.assert(self.vector, self.type.Node);
                end,                
                ["Type argument list reflect original argument."] = function(self)
                    Test.assert(self.typeArgs, self.type.TypeArguments);
                end,                
            }
        },
        {
            name = "Creating a Type representing Event<EventId>::Message<std::string, GfxData>.",
            init = function(self)    
                mixinContext(self); 
                self.typeArg1 = TypeArgument.New(
                        self.event,
                        NodeList.New{ self.eventId } 
                    );                
                self.typeArg2 = TypeArgument.New(
                        self.message,
                        NodeList.New{ self.stdstring, self.gfxData } 
                    );                
                self.typeArgs = TypeArgumentList.New{self.typeArg1, self.typeArg2};
                self.type = Type.New(self.message, self.typeArgs);                
            end,
            tests = {
                ["Type points to Node representing Event::Message."] = function(self)
                    Test.assert(self.message, self.type.Node);
                end,                
                ["Type argument list reflect original argument."] = function(self)
                    Test.assert(self.typeArgs, self.type.TypeArguments);
                end,                
            }
        },       
    }    
}); -- End of register call
