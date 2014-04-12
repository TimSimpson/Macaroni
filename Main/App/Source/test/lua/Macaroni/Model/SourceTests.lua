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
local FileName = require "Macaroni.Model.FileName";
local Path = require "Macaroni.IO.Path";
local Source = require "Macaroni.Model.Source";

Test.register(
{
    name = "Source Tests",
    tests = {
        {
            name = "Creating a source object.",
            init = function(this)
                this.file = FileName.Create(Path.New("", "blah"));
                Test.assertEquals(1, this.file.ReferenceCount);
                this.src = Source.Create(this.file, 27, 1);
            end,
            tests = {
                ["Column should return the given column."] = function(this)
                    Test.assertEquals(1, this.src.Column);
                end,
                ["FileName should return the given file."] = function(this)
                    Test.assertEquals(this.file, this.src.FileName);
                end,
                ["Line should return the given line."] = function(this)
                    Test.assertEquals(27, this.src.Line);
                end,
                ["ReferenceCount on the File should have been incremented."] = function(this)
                    local refCount = this.file.ReferenceCount
                    print(refCount)
                    if (refCount ~= 2 and refCount ~= 3) then
                        -- Can be either two or three, varies
                        -- randomly.
                        Test.assertEquals(3, this.file.ReferenceCount);
                    end
                end,
                ["ReferenceCount on the Source starts as one."] = function(this)
                    Test.assertEquals(1, this.src.ReferenceCount);
                end,
                ["ToString lookz like dis:"] = function(this)
                    Test.assertEquals("blah, line 27, column 1", tostring(this.src));
                end,
            }
        }
    }
}); -- End of register call
