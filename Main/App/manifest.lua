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

require "Macaroni.IO.Path"
Path = Macaroni.IO.Path;

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.App",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Macaroni is a builder / parser for C++ that aims to increase
    DRYness by slimming down boilerplate and eliminating the need to store
    redundant information.
]]

-- Bug- the Boost Build builder only looks at the last source path.
sources = { "Source/main/lua", "Source/main/resources", "Source/main/mcpp",
            --TODO: Put back in when using 0.1.0.20: "Source/test/mcpp" 
          }
output = "GeneratedSource"
--releasePath = "../" .. properties.macaroni.releasepath .. "/App";

-- Don't trust Macaroni's own dependencies for these, not yet at least.
--dependency { group="Macaroni", name="Boost", version="1.39.0-0" }
--dependency { group="Macaroni", name="CppStd", version="1.0.0.0" }
dependency {group="Macaroni", name="Boost-filesystem", version="1.46.1"}
dependency {group="Macaroni", name="Boost-regex", version="1.46.1"}
dependency {group="Macaroni", name="Lua", version="5.1.4"}
        
function generate()    
	run("CheckModel");
	run("LuaGlue", { luaImportCode =[[ 
	extern "C" 
	{	
		#include <lauxlib.h>
		#include <lualib.h>
	}
	]] });
	run("HtmlView");
    run "Cpp"
    --runGenerator "Generators/LuaGlue"
    run "InterfaceMh"
    run "JamGenerator"
    run "VersionInfoGenerator"       
end
   
jamArgs = 
{ 	
	ExcludePattern = "Main.cpp *Test.cpp *Tests.cpp .svn",
	ExtraTargets = [[		
		install installmsgs
			:	../Source/main/resources/Messages.txt
			:	<variant>debug:<location>release/debug
				<variant>release:<location>release/release
			;

	  	exe macaroni 
	  		:	library #library_sources
	  			#library_dependencies
	  			../Source/main/mcpp/Main.cpp
	  			../Source/main/resources/Macaroni.rc
            :
				
	  		;
	  		
	  	install final 
        :	macaroni		       
        :	<install-dependencies>on <install-type>EXE
          #<install-type>LIB
          <variant>debug:<location>release/debug
          <variant>release:<location>release/release          		
        ;
	  ]],
	  Link="static",
	  -- Alas, not yet... 
	  -- Tests = 
	  -- {
	  --   "Macaroni/Build/ManifestOrganizerTest.cpp",
	  --   "Macaroni/IO/GeneratedFileWriterTest.cpp",
	  --   "Macaroni/Model/ContextTests.cpp",
	  --   "Macaroni/Model/NodeTest.cpp",
	  --   "BoostTestTest.cpp",	    
	  -- }
	};
		
function build()	
	run("BoostBuild", jamArgs)
end

function test()
	local currentPath = Path.New(manifestDirectory);
	local exePath = currentPath:NewPathForceSlash(
		output .. "/release/debug/macaroni.exe");
	local luaTestsPath = currentPath:NewPathForceSlash(
		"Source/test/lua");
	local cmd = exePath.AbsolutePath .. " luaTests " .. luaTestsPath.AbsolutePath;
	print("Running Lua tests:");
	print(cmd);
	os.execute(cmd);
end

function install()	
end
