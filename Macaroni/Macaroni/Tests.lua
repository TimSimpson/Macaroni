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
        print("[" .. output.getFullSuiteName() .. '] "' .. currentTest .. '"');        
        print(msg);
        print();
        print("-------------------------------------------------------------------------------");
        print();
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
