require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Regression Tests- Lua Glue",
    version="1.0.0.0"
}

description='Tests the generator "LuaGlue."'

sources = { "Source" }
output = "GeneratedSource"

--dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}
dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group = "Lua", name = "Lua", version = "5.1.4" }

function generate()
    print "Greetings from the manifest of the LUA test.\n\n"
    --print("Lua 5.1.4 source path is " .. properties.lua["5.1.4"].source)
    run("LuaGlue", { luaImportCode =[[ 
extern "C" 
{	
	#include <lauxlib.h>
	#include <lualib.h>
}
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
	ExcludePattern = "Main.cpp .svn",
	ExtraTargets = [[
	  	exe LuaTest 
	  		:	libSources
	  			../Source/Main.cpp
	  		;
	  ]]
	};
		
function build()
	print("I is build method.\n");
	run("BoostBuild", jamArgs)
end

function install()
	print("Yo yo yo!  I is an installer!");
	run("BoostBuild", jamArgs)
	--runInstaller("JamGenerator");
end

