require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. "LuaGlue",
    version=upper.Version,
}

description='Tests the generator "LuaGlue."'

sources = { "Source" }
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.52"}
dependency {group="Macaroni", name="CppStd", version="2003"}
--dependency {group = "Lua", name = "Lua", version = "5.1.4" }
dependency {group = "Macaroni", name = "Lua", version = "5.1.4" }

function generate()
    print "Greetings from the manifest of the LUA test.\n\n"
    print("Lua 5.1.4 source path is " .. properties.lua["5.1.4"].source)
    print("Boost crap is at " .. properties.boost["1.52"].include)
    run("LuaGlue", { luaImportCode =[[
	#include <lauxlib.h>
	#include <lualib.h>
]] });
    run("HtmlView");
    run("Cpp");
    --run "JamGenerator"
    run("InterfaceMh");
    run("ShowLibs");
    --local rtnCode = os.execute("bjam")
    --print("BJAM return code = " .. rtnCode .. ".")
    --if (rtnCode ~= 0) then
    --    --error("Looks like the plan's not going swell.")
    --end
end

jamArgs =
{
	ExcludePattern = "Main.cpp Test.cpp .svn",
	ExtraTargets = [[
	    # Same old problem! Can't load the boost test stuff as a shared library.

		#lib boost_unit_test_framework :
		#	: <name>boost_unit_test_framework
		 # 	<search>"]] .. properties.boost.current["path"]
	    	      .. [[/stage/lib"
	    #	      ;


	 	#alias test_dependencies
	#    	: "]] .. properties.boost.current["path"]
	   	      .. [[/libs/test/build//boost_unit_test_framework"
	  #      :
	   # 	;

		#unit-test test
		#    : # boost_unit_test_framework
		#    test_dependencies
		#    #library
		#
		#      ../Source/Test.cpp
		#      : <search>"]] .. properties.boost.current["path"]
	   	      .. [[/stage/lib"
		#    ;

		#unit-test test
		#    : library
		#      test_dependencies
		#      ../Source/Test.cpp
		#    ;

	  	exe Main
	  		:	library #library_sources
	  			#library_dependencies
	  			../Source/Main.cpp
	  		;
	  ]],
	  Shared = True,
	  Tests = {"Test.cpp"}
	};

function build()
	require "Macaroni.IO.Path"
	Path = Macaroni.IO.Path;
	filePath = Path.New(manifestDirectory):NewPathForceSlash("LuaTest.vcproj")

	print("__FILE__ == " .. tostring(filePath))
	-- run("Vcpp/Vcpp", {
	-- 	    ProjectFile=filePath.AbsolutePath,
	-- 		ProjectGUID="FD133633-4AB0-4068-BDD9-A2A343751345",
	-- 		RootNamespace="Macaroni::Tests::Lua",
	-- 		PlatformNames={"Win32"}
	-- 	})

	run("BoostBuild", jamArgs)
end

--function test()
--	run("BoostBuild", { CmdLine="test"} );
--end

function install()
	print("Yo yo yo!  I is an installer!");
	run("BoostBuild", jamArgs)
	--runInstaller("JamGenerator");
end

