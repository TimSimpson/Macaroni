--------------------------------------------------------------------------------
-- Copyright 2012 Tim Simpson
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

--------------------------------------------------------------------------------
-- This file contains code which is read by every project environment on
-- start-up.
--------------------------------------------------------------------------------

old_print = print
print = function(msg)
    output:WriteLine(tostring(msg))
end

local Files = Macaroni.Model.Project.Files
local FileSet = Macaroni.IO.FileSet;
local Path = Macaroni.IO.Path;
local PathList = Macaroni.IO.PathList;

filePath = function(relP, path2)
    -- Returns a Path relative to the working directory.
    if path2 ~= nil then
        local p = Path.New(relP)
        return p:NewPathForceSlash(path2)
    end
    local p = Path.New(getWorkingDirectory());
    return p:NewPathForceSlash(relP);
end

fileSet = function(path, regex)
    -- Returns a file set relative to the working directory.
    regex = regex or ""
    return FileSet.CreateRelative(filePath(path), regex)
end

files = Files.New

pathList = function(array)
    local rtnValue = PathList.New()
    for i = 1, #array do
        rtnValue[#rtnValue + 1] = filePath(array[i]);
    end
    return rtnValue;
end


