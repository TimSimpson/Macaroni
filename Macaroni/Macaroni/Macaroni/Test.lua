
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
                        i .. '": Assert msg was:"' .. err);
                end
            end
            for i,v in pairs(actual) do
                if (expected[i] == nil) then
                    error('ASSERT FAILED: Found an extra table value with key="' ..
                        tostring(i) .. '", value="' .. tostring(v) .. 
                        '" which was unexpected.');
                end                
            end
        elseif (expected ~= actual) then 
            error("ASSERT FAILED: Expected:" .. tostring(expected)
                  .. ",Actual:" .. tostring(actual), 2);
        end
    end,
    
    assertFalse = function (expected, actual)
        if (type(expected) ~= type(actual)) then
            error("ASSERTFALSE FAILED: Expected Type:" .. tostring(type(expected))
                  .. ",Actual:" .. tostring(type(actual)), 2);
        end
        if (expected == actual) then 
            error("ASSERTFALSE FAILED: Expected:" .. tostring(expected)
                  .. ",Actual:" .. tostring(actual), 2);
        end
    end,
    
    failures = {},
    
    register = function (object) 
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
        for i, v in pairs(collection) do
            Test.runCase(test, i, v, output);
        end
    end,
    
    run = function (test, output)
        output.setSuite(test.name);
        if (test.init ~= nil) then
           Test.runCaseList(test, test.init, output);
        end	
        Test.runCaseList(test, test.tests, output);
        if (test.shutdown ~= nil) then
            Test.runCaseList(test, test.shutdown, output);
        end
    end,
    
    runAll = function (output)
        for i, v in pairs(Test.testList) do
            Test.run(Test.testList[i], output);
        end
        Test.testList = {}
    end,
    
    testList = {};
};

