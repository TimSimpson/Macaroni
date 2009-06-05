
Test = {
    assert = function (expected, actual)
        if (type(expected) ~= type(actual)) then
            error("ASSERT FAILED: Expected Type:" .. tostring(type(expected))
                  .. ",Actual:" .. tostring(type(actual)), 2);
        end
        if (type(expected) == "table") then
            for i,v in pairs(expected) do
                local status, err = pcall(function() 
                    assert(expected[i], actual[i]) 
                end);
                if (not status) then
                    error('ASSERT FAILED: While checking table key "' ..
                        i .. '": Assert msg was:"' .. err, 2);
                end
            end
            for i,v in pairs(actual) do
                if (expected[i] == nil) then
                    error('ASSERT FAILED: Found an extra table value with key="' ..
                        tostring(i) .. '", value="' .. tostring(v) .. 
                        '" which was unexpected.', 2);
                end                
            end
        elseif (expected ~= actual) then 
            error("ASSERT FAILED: Expected:" .. tostring(expected)
                  .. ",Actual:" .. tostring(actual), 2);
        end
    end,
    
    assertFalse = function (expected, actual)
        --[[if (type(expected) ~= type(actual)) then
            error("ASSERTFALSE FAILED: Expected Type:" .. tostring(type(expected))
                  .. ",Actual:" .. tostring(type(actual)), 2);
        end]]--
        if (expected == actual) then 
            error("ASSERTFALSE FAILED: Expected:" .. tostring(expected)
                  .. ",Actual:" .. tostring(actual), 2);
        end
    end,
    
    failures = {},
    
    -- Expects object to be a valid test suite.
    register = function (object)  
        if (object.name == nil) then
            error("Given test suite has no name property.", 2);
        end       
        if (object.tests == nil) then
            error("Given test suite " .. object.name .. " has no tests property.", 2);
        end
        --[[if (#object.tests < 1) then
            error("Given test suite  " .. object.name .. " has " .. #object.tests .. " tests.");
        end ]]--
        Test.testList[object.name] = object;
    end,
    
    runCase = function (test, name, func, output)
        output.setTest(name);		
        co = coroutine.create(function() func(test) end);
        result, msg = coroutine.resume(co);
        if result ~= true then
            failure = { name = name, cause = msg }; -- .. debug.traceback(co) };
            Test.failures[#Test.failures + 1] = failure;                     
            output.fail(failure.cause);            
        else
            output.pass("Success");
        end      
    end,
    
    runCaseList = function(test, collection, output)
        if (type(collection) ~= "table") then
            error("Table expected for arg #2.", 2);
        end
        for i, v in pairs(collection) do
            if (type(v) == "function") then
                Test.runCase(test, i, v, output);
            end
            if (type(v) == "table") then
                Test.runSuite(v, output);
            end            
        end
    end,
    
    runAll = function (output)
        for i, v in pairs(Test.testList) do
            Test.runSuite(Test.testList[i], output);
        end
        Test.testList = {}
    end,
    
    runSuite = function (test, output)
        if (test.name == nil) then
            error("No name property for test suite.", 2);
        end       
        if (test.tests == nil) then
            error("Test suite " .. test.name .. " has no tests property.", 2);
        end
        output.enterSuite(test.name);
        if (test.init ~= nil) then
           Test.runCase(test, "init", test.init, output);
        end	
        Test.runCaseList(test, test.tests, output);
        if (test.shutdown ~= nil) then
            Test.runCase(test, "shutdown", test.shutdown, output);
        end
        output.exitSuite(test.name);
    end,
    
    testList = {};
};

