require "Macaroni/Test";

require "Macaroni.Model.Context"
--require "Macaroni.Model.Document"

require "Macaroni/Model/AxiomTests";
--require "Macaroni/Model/ClassTests";
require "Macaroni/Model/ContextTests";
-- Commenting out Document Tests... FOR NOW...
--require "Macaroni/Model/DocumentTests";
require "Macaroni/Model/FileNameTests";
--require "Macaroni/Model/NamespaceTests";
require "Macaroni/Model/NodeTests";
require "Macaroni/Model/ReasonTests";
require "Macaroni/Model/SourceTests";
require "Macaroni/Parser/Cpp/CppParserTests";

local currentTest;
suiteNames = {};
output = {
    enterSuite = function(testName)
        suiteNames[#suiteNames + 1] = testName;
        --print("[" .. currentSuite .. "]");
    end,
    
    exitSuite = function(testName)
        if (suiteNames[#suiteNames] ~= testName) then
            error("A suite at some point wasn't exitted correctly.");
        end
        suiteNames[#suiteNames] = nil;
    end,
    
    fail = function(msg)
        -- Lua error msg is in form: filename.lua:line#: error
        -- Change it to VS2008 compatable form: filename.lua(line#): error in blah
        local index1 = string.find(msg, ".lua:", 0, true);
        local index2 = string.find(msg, ":", index1 + 5, true) + 1;
        local fileName = string.sub(msg, 0, index1 + 3);
        local lineNumber = string.sub(msg, index1 + 5, index2 - 2);
        local errorMsg = string.sub(msg, index2, #msg);
        local newMsg = fileName .. "(" .. lineNumber .. ") : error at line " .. lineNumber 
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
print("Failures: " .. #Test.failures );
