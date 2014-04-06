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


if MACARONI_VERSION == "0.1.0.27" then
    Files = Macaroni.Model.Project.Files
    FileSet = Macaroni.IO.FileSet;
    Path = Macaroni.IO.Path;
    PathList = Macaroni.IO.PathList;
else
    Files = require "Macaroni.Model.Project.Files"
    FileSet = require "Macaroni.IO.FileSet"
    Path = require "Macaroni.IO.Path"
    PathList = require "Macaroni.IO.PathList"
end



loadBoostLib = function(name)
    -- Short hand for loading a Boost Library.
    if not properties.boost or not properties.boost.version then
        error("Can't loadBoostLib - properties.boost.version not set in init.lua.");
    end
    return load("Macaroni", "Boost-" .. name, properties.boost.version)
           :Target("lib");
end

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

local createPathList = function(array, elementTransformer)
    local rtnValue = PathList.New()
    for i = 1, #array do
        local element = array[i]
        if elementTransformer then
            element = elementTransformer(element)
        end
        rtnValue[#rtnValue + 1] = element;
    end
    return rtnValue;
end

pathList = function(array)
    return createPathList(array, filePath);
end

pathList2 = function(array)
    return createPathList(array)
end
