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
require "Macaroni.Model.FileName";

local FileName = Macaroni.Model.FileName;

Test.register(
{	
    name = "FileName Tests",    
    tests = {    
        ["Creating a file."] = function(this)
            local file = FileName.Create("blah");
            Test.assert("blah", tostring(file));
        end,
        ["Reference counting"] = function(this)
            local file = FileName.Create("blah");            
            Test.assert(1, file.ReferenceCount);
            local file2 = file; --lua makes no copy.
            Test.assert(1, file.ReferenceCount);
        end,
    }    
}); -- End of register call
