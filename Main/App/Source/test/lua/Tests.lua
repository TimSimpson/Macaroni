require "Macaroni/Test";

--[[ 
Requiring C++ modules before LUA files seems to cause some insanely nasty
memory errors as of 2009-10-03.  I can't determine the source of these
errors, but they go away when you don't require the C++ modules first.

They occur when calling realloc- after changing the call to a trio of malloc,
memcpy, and free I discovered the same error occuring on memcpy.  My guess
is that somehow my code to register C++ modules is wrong and is storing
something in Lua's memory space that is freaking out during an attempt to
move it.

I'm not sure what this could be.  I recently added an extra glue function
for NodeLists, but aside from that it seems to only be loading things that have
had Lua glue for awhile now.  

Scrary!  I don't know what it could be.
]]--

--require "Macaroni.Model.Context"
--require "Macaroni.Model.Document"

require "Macaroni/Model/AttributeTests";
require "Macaroni/Model/AxiomTests";
require "Macaroni/Model/Cpp/ClassTests";
require "Macaroni/Model/ContextTests";
-- Commenting out Document Tests... FOR NOW...
-- require "Macaroni/Model/DocumentTests";
require "Macaroni/Model/FileNameTests";
--require "Macaroni/Model/NamespaceTests";
require "Macaroni/Model/NodeTests";
require "Macaroni/Model/NodeListTests";
require "Macaroni/Model/ReasonTests";
require "Macaroni/Model/SourceTests"; 
require "Macaroni/Model/TypeTests"; 
require "Macaroni/Model/TypeArgumentTests"; 
-- Bug below?
require "Macaroni/Model/TypeArgumentListTests";  -- suspect
require "Macaroni/Model/TypeListTests"; 
--require "Macaroni/Parser/Cpp/Tests/Namespaces";

require "Macaroni/Parser/Pippy/Tests/Index"; -- primary suspect

-- Generator tests.
-- require "Generators/LuaGlueTest";

local currentTest;
suiteNames = {};
output = {
    enterSuite = function(testName)
        suiteNames[#suiteNames + 1] = testName;
        --print("[" .. testName .. "]\n");
    end,
    
    exitSuite = function(testName)
        if (suiteNames[#suiteNames] ~= testName) then
            error("A suite at some point wasn't exitted correctly.");
        end
        suiteNames[#suiteNames] = nil;
    end,
    
    fail = function(msgArg)
        local msg = nil;
        if (type(msgArg) == "string") then
            msg = msgArg;
        else
            local status, err = pcall(function() 
                msg = tostring(msgArg);
            end);
            if (not status) then
                msg = "An error occured while calling tostring on the error object to report what the error was!";
            end
            if (msg == nil) then
                msg = "{Error message was nil.}";
            end
        end
       
        -- Lua error msg is in form: filename.lua:line#: error
        -- Change it to VS2008 compatable form: filename.lua(line#): error in blah
        local index1 = string.find(msg, ".lua:", 0, true);
        local errorMsg = nil;
        local prefix = nil;
        if (index ~= nil) then
            local index2 = string.find(msg, ":", index1 + 5, true) + 1;
            local fileName = string.sub(msg, 0, index1 + 3);
            local lineNumber = string.sub(msg, index1 + 5, index2 - 2);
            errorMsg = string.sub(msg, index2, #msg);
            prefix = fileName .. "(" .. lineNumber .. ") : error at line " .. lineNumber;
        else
            prefix = ""; 
            errorMsg = msg;       
        end
        local newMsg = prefix 
            .. " [" 
            .. output.getFullSuiteName() .. '] "' .. currentTest .. '":' .. errorMsg;                
        --print("[" .. output.getFullSuiteName() .. '] "' .. currentTest .. '"');        
        --print(msg);
        --print();
        --print("-------------------------------------------------------------------------------");
        --print();
        print(newMsg);
    end,
    
    getFullSuiteName = function()
        local suiteName = "";
        for i=1, #suiteNames do
            suiteName = suiteName .. "[" .. suiteNames[i] .. "]";
        end
        return suiteName;
    end,
    
    pass = function(str)
        --print(currentSuite .. " [" .. currentTest .. "] ... :)");
    end,
    
    setTest = function(testName)
        currentTest = testName;
    end,    
};

Test.runAll(output);

print("Reached the end of all tests without dying.  Still alive, I'm still alive...");

print();
print("Tests   : " .. Test.count);
print("Failures: " .. #Test.failures );

if (#Test.failures > 0) then
    error("Experienced failures running Lua tests.");
end