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
            --"Source/test/mcpp"
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
	print("Checking the model...")
	run("CheckModel");
    run("ReplCommand");
    print("Wrapping things in LuaGlue...")
	run("LuaGlue", { luaImportCode =[[
	extern "C"
	{
		#include <lauxlib.h>
		#include <lualib.h>
	}
	]],
    standardIncludes = [[
        #include <Macaroni/Exception.h>
    ]],
    luaCatchCode=[[ \
        } catch(const Macaroni::Exception & ex) { \
            return luaL_error(L, ex.message().c_str()); \
        } catch(const std::exception & ex) { \
            return luaL_error(L, ex.what()); \
        }
    ]] } );
	-- print("Creating HTML View...")
	run("HtmlView");
	print("Generating C++ code...")
    run "Cpp"

    print("Creating Macaroni Library Header...")
    run "InterfaceMh"
    print("Writing Boost Build files...")
    run "JamGenerator"
    print("Creating library version info file...")
    run "VersionInfoGenerator"
end

jamArgs =
{
	-- Uncomment the line below to see the commands Boost Build is using:
	--CmdLine = [[ -d+2 ]],
	ExcludePattern = "Main.cpp *Test.cpp *Tests.cpp .svn",
	ExtraTargets = [[
        #alias test_dependencies
        #    : "]] .. properties.boost.current["path"] ..
            [[/libs/test/build//boost_unit_test_framework"
        #    :
        #    ;

		install installmsgs
			:	../Source/main/resources/Messages.txt
			:	<variant>debug:<location>release/debug
				<variant>release:<location>release/release
			;

        #unit-test macaroni-tests
        #    :   library
        #        test_dependencies
        #        ../Source/test/mcpp/Macaroni/Model/AnnotationValueTest.cpp
        #    :  <target-os>windows:<linkflags>/LIBPATH:"]]
            .. properties.boost.current["path"] .. [[/stage/lib"
        #
        #    ;

	  	exe macaroni
	  		:	library #library_sources
	  			#library_dependencies
	  			../Source/main/mcpp/Main.cpp
	  			../Source/main/resources/Macaroni.rc
            :  <target-os>windows:<linkflags>/LIBPATH:"]]
            .. properties.boost.current["path"] .. [[/stage/lib"

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
    print("Creating Visual C++ 9 Project File...")
    local proj = {
        ProjectFile="macaroni_9.vcproj",
        ProjectGUID="5D09EE96-A873-4C96-813E-B48BBE31EB10",
        RootNamespace="Macaroni",
        SourcePaths = {"Source/main/resources", "Source/main/mcpp"},
        AdditionalIncludePaths={
            properties.boost.current["path"]
        },
        AdditionalLibraryPaths={
            properties.boost.current["path"] .. '/stage/lib'
        },

        AdditionalSourcePaths={
            [[C:\Users\Tim\Macaroni\Libraries\Macaroni\Lua\5.1.4\Cpp]]
        },
        PreBuildEvent = [[macaroni.exe generatorPath F:\Work\Lp3\Projects\Macaroni\Code\trunk\Main\Generators generate]],
        SourceExcludePatterns = {
            "lua.cpp",
            "lu.cpp.cpp",
            "Test.cpp", "Tests.cpp"
        }
    };
    run("VCpp/VCpp9", proj)

    print("Creating Visual C++ 10 Project File...")
    proj.ProjectFile="macaroni_10.vcxproj";
    proj.AdditionalSourcePaths={
        [[C:\Documents and Settings\Tim\Macaroni\Libraries\Macaroni\Lua\5.1.4\Cpp]]
    };
    run("VCpp/VCpp10", proj)

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
