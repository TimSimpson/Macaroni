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

function Generate(library, path)
    print "Generating build.jam for the Generated source directory.\n";
    local buildjam = path:NewPath("/build.jam");
    print("Opening " .. buildjam.AbsolutePath .. "\n");   
    local writer = buildjam:CreateFile();
    writer:Write([[
# Generated by Jam Generator.
alias generated
	:	[ glob-tree *.c	: .svn ]
		[ glob-tree *.cpp	: .svn ]
	:	<include>./
		<include>../Source
	;
alias pureCppSource
	:	[ glob-tree *.c	: .svn ]
		[ glob-tree *.cpp	: .svn *Test.cpp Main.cpp ]
		[ glob-tree *.h	: .svn ]				
		[ glob-tree *.hpp	: .svn ]
	;
    ]]);    
end