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

id =
{
    group="Macaroni" ,
    name="Macaroni.All",
    version="0.1.0.25",
    author="Tim Simpson"
}

description= [[
    Builds the development version of Macaroni named "Cavatappi" before running
    all tests.
    Rebuilds Macaroni from pure C++ source to ensure it is identical, then
    creates the Macaroni website.
]]


children =
{
  "Main",
  --"Generators",
  --"Libraries"
  "Tests",
  "Release",
};

function generate()
end

function build()

end

function install()
end
