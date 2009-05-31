require "Macaroni/Test";

require "Macaroni.Model.Context"
require "Macaroni.Model.Document"

doc = Macaroni.Model.Document.New("test23");
doc:Read("namespace A::B{}");        
doc:Read("namespace A::B{}");

require "Macaroni/Model/ClassTests";
require "Macaroni/Model/ContextTests";
-- Commenting out Document Tests... FOR NOW...
require "Macaroni/Model/DocumentTests";
require "Macaroni/Model/NamespaceTests";
require "Macaroni/Model/NodeTests";

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
        print("[" .. getFullSuiteName() .. '] "' .. currentTest .. '"');        
        print(msg);
        print();
        print("-------------------------------------------------------------------------------");
        print();
    end,
    
    getFullSuiteName = function()
        local suiteName = "";
        for name in suiteNames do
            suiteName = suiteName .. "[" .. name .. "]";
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
