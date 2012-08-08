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

require "os"
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function Generate(library, path)
    print "Generating build.jam for the Generated source directory.\n";
    local buildjam = path:NewPath("/jamroot.jam");
    print("Opening " .. buildjam.AbsolutePath .. "\n");
    local writer = buildjam:CreateFile();
    writer:Write(
[[################################################################################
## Copyright 2011 Tim Simpson
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##    http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
################################################################################

# Root jam file for "pure" C++ version of Macaroni

using boost 
     : 1.49.0
     : <include>/C:/Users/Tim/Tools/Local/Boost/boost_1_49_0
       <library>C:/Users/Tim/Tools/Local/Boost/boost_1_49_0/stage/lib
     ; 
     
import boost ;
boost.use-project 1.49.0 ;


import path ;


lib lua_lib
    : [  path.glob-tree "../../Dependencies/Lua/Target" : *.cpp : lua.cpp lu.cpp.cpp  ]
    ;
    
alias sources
    :   [ path.glob-tree ../Source/main/mcpp : *.c ]
        [ path.glob-tree ../Source/main/mcpp : *.cpp ]
        #[ path.glob-tree ./ : *.cpp ]        
        lua_lib
        /boost//filesystem
        /boost//regex
        /boost//system
    :   <include>./
        <include>../Source/main/mcpp
    ;

exe macaroni_p  # In linux it can't create the file because the name is the
                # same as a directory.        
    :   [ path.glob-tree ../Source/main/mcpp : *.c ]
        [ path.glob-tree ../Source/main/mcpp : *.cpp ]
        #[ path.glob-tree ./Macaroni : *.cpp ]        
        lua_lib
        ../Source/main/resources/Macaroni.rc
        /boost//filesystem
        /boost//regex
        /boost//system
    :   <include>./
        <include>../Source/main/mcpp
        <target-os>linux:<define>MACARONI_COMPILE_TARGET_LINUX
        <target-os>windows:<define>MACARONI_COMPILE_TARGET_WINDOWS
    ;

    ]]);
end

function Build(a, b, c, d)
  os.execute("bjam GeneratedSource -d+2")
end
