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

Test = {
    assertEquals = function (expected, actual)
		-- Checks if "expected" is equal to "actual" and if they aren't calls
		-- error. The advantage of using this as opposed to 
		-- "assert(expected == actual)" is that the error message is nicer.
        if (type(expected) ~= type(actual)) then			
            error("ASSERT FAILED: Expected Type:" .. tostring(type(expected))
                  .. ",Actual:" .. tostring(type(actual)), 2);
        end
        if (type(expected) == "table") then
            for i,v in pairs(expected) do
                local status, err = pcall(function() 
                    Test.assertEquals(expected[i], actual[i]) 
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
        
    assertNotEquals = function (expected, actual)
		-- Tests that the two objects are not equal.
        --[[if (type(expected) ~= type(actual)) then
            error("ASSERTFALSE FAILED: Expected Type:" .. tostring(type(expected))
                  .. ",Actual:" .. tostring(type(actual)), 2);
        end]]--
        if (expected == actual) then 
            error("ASSERT NOT EQUALS FAILED: Expected:" .. tostring(expected)
                  .. ",Actual:" .. tostring(actual), 2);
        end
    end,
    
    count = 0,  -- Stores a count of all tests.
    
    failures = {},  -- Stores a list of all failures.
        
    register = function (object)  
        -- Stores the given test case into its list of tests. The name of the
        -- test must be unique.
		-- Expects object to be a valid test suite.
        if (object.name == nil) then
            error("Given test suite has no name property.", 2);
        end       
        if (object.tests == nil) then
            error("Given test suite " .. object.name .. " has no tests property.", 2);
        end
        --[[if (#object.tests < 1) then
            error("Given test suite  " .. object.name .. " has " .. #object.tests .. " tests.");
        end ]]--
        if testList[object.name] ~= nil then
			error("The given test suite " .. object.name .. " has already been registered!");
        end
        Test.testList[object.name] = object;
    end,
    
    runCase = function (test, name, func, output)
		-- Runs a test case.
        output.setTest(name);		
        co = coroutine.create(function() func(test) end);
        result, msg = coroutine.resume(co);
        Test.count = Test.count + 1;
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
                if (v.name == nil) then
                    v.name = i; -- assign name property using index name if none found.
                end
                Test.runSuite(v, output);
            end            
        end
    end,
    
    runAll = function (output)
		-- Runs all tests that were previously registered.
        for i, v in pairs(Test.testList) do
            Test.runSuite(Test.testList[i], output);
        end
        Test.testList = {}
    end,
    
    runSuite = function (test, output)
		-- Runs a "suite" instead of a test case.  Runs "init" and 
		-- "shutdown" methods last.
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

