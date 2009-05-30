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
local currentSuite;
output = {
    setSuite = function(testName)
        currentSuite = testName;
        --print("[" .. currentSuite .. "]");
    end,
    setTest = function(testName)
        currentTest = testName;
    end,
    fail = function(msg)
        print("[" .. currentSuite .. '] "' .. currentTest .. '"');        
        print(msg);
        print();
        print("-------------------------------------------------------------------------------");
        print();
    end,
    pass = function(str)
        --print(currentSuite .. " [" .. currentTest .. "] ... :)");
    end
};

Test.runAll(output);

print("Reached the end of all tests without dying.  Still alive, I'm still alive...");

print();
print("Failures: " .. #Test.failures );
