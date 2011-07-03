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


function aboutText(version)
	local about = [[
Macaroni for C++, Version ]] .. version .. "\n" .. [[
----------------------------------

Copyright 2011 Tim Simpson                                                   

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

Macaroni incorporates the following dependencies:


The Programming Language Lua
----------------------------

Copyright (C) 1994-2011 Lua.org, PUC-Rio.
Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.

Boost C++ Libraries
-------------------

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
]]			
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
    createAbout(path, library)
end

function createAbout(path, library)
	local version = library.Version;    
	local file = path:NewPath("/Macaroni/AboutText.h");
    local writer = file:CreateFile();
	writer:Write(aboutText(version));
end

function createVersionNoH(path, library)
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
