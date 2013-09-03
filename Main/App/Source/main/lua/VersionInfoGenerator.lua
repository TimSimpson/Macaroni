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
require "About";


function aboutTextCString(version)
    local about = aboutText(version);
	local aboutT = string.gsub("~ " .. about, "\"", "\\\"")
	local aboutT = string.gsub(aboutT, "\n", "\\n\"\n\"~ ");
	return [[
"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
"]] .. aboutT .. [[\n"
"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
	]];
end

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
    createVersionNoH(path, library)
    createAboutTextH(path, library)
    createAboutTxt(path, library)
end

function GetMethod(name)
    if name == "Generate" then
        return
        {
            Run = function(args)
                createVersionNoH(args.path, args.projectVersion)
                createAboutTextH(args.path, args.projectVersion)
                createAboutTxt(args.path, args.projectVersion)
            end,
        };
    end
end

function createAboutTxt(path, library)
    local version = library.Version;
    local file = path:NewPathForceSlash("LICENSE.txt");
    local writer = file:CreateFile();
    writer:Write(aboutText(version));
end

function createAboutTextH(path, library)
	local version = library.Version;
	local file = path:NewPath("/Macaroni/AboutText.h");
    local writer = file:CreateFile();
	writer:Write(aboutTextCString(version));
end

function createVersionNoH(path, library)
	local commaVersion = string.gsub(library.Version, "%.", "\,");
    local version = library.Version;
    local file = path:NewPath("/Macaroni/VersionNo.h");
    local writer = file:CreateFile();

    writer:Write([[
#ifndef MACARONI_VERSION

#define MACARONI_VERSION ]] .. commaVersion .. "\n" .. [[
#define MACARONI_VERSION_STRING "]] .. version .. "\"\n" .. [[
#define MACARONI_FILE_DESCRIPTION "Macaroni for C++"
#define MACARONI_COPYRIGHT "(C) Tim Simpson, 2011"
#define BUILD_TIMESTAMP_LOCAL "]] .. localTime() .. "\"\n" .. [[
#define BUILD_TIMESTAMP_UTC "]] .. utc() .. "\"\n" .. [[
#endif
]]);

end
