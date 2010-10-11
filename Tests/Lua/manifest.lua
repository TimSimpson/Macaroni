require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Regression Tests- Lua Glue",
    version="1.0.0.0"
}

description='Tests the generator "LuaGlue."'

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost", version="1.39.0-0"}
dependency {group="Macaroni", name="CppStd", version="1.0.0.0"}


function generate()
    print "Greetings from the manifest of the LUA test.\n\n"
    runGenerator("LuaGlue", { luaImportCode =[[ 
extern "C" 
{	
	#include <lauxlib.h>
	#include <lualib.h>
}
]] });

    runGenerator("HtmlView");
    runGenerator("Cpp");
    runGenerator "JamGenerator"
    local rtnCode = os.execute("bjam")
    print("BJAM return code = " .. rtnCode .. ".")
    if (rtnCode ~= 0) then
        --error("Looks like the plan's not going swell.")
    end
end


