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
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function formatTimeToString(date)
    return (date.year .. "-" .. date.month .. "-" .. date.day .. "-" .. 
            date.hour .. ":" .. date.min .. ":" .. date.sec);
end

function localTime()
    return formatTimeToString( os.date("*t") );
end

function utc()
    return formatTimeToString( os.date("!*t") );
end

function Generate(library, path)    
    local commaVersion = string.gsub(library.Version, "%.", "\,");
    local version = library.Version;    
    local file = path:NewPath("/Macaroni/VersionNo.h");
    local writer = file:CreateFile();

    writer:Write([[
#ifndef MACARONI_VERSION

#define MACARONI_VERSION ]] .. commaVersion .. [[ 
#define MACARONI_VERSION_STRING "]] .. version .. [["
#define MACARONI_FILE_DESCRIPTION "Macaroni for C++"
#define MACARONI_COPYRIGHT "(C) Tim Simpson, 2011"
#define BUILD_TIMESTAMP_LOCAL "]] .. localTime() .. [["
#define BUILD_TIMESTAMP_UTC "]] .. utc() .. [["
#endif
]]);    

end
