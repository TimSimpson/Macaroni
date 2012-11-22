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
require "Macaroni.IO.Path";
require "Macaroni.Model.Source";

local FileName = Macaroni.Model.FileName;
local Path = Macaroni.IO.Path;
local Source = Macaroni.Model.Source;

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
                    Test.assertEquals(2, this.file.ReferenceCount);
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
