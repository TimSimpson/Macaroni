require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Tests- Language Features",
    version="1.0.0.0"
}

-- Life cycle:
-- Read file.
-- For each dependency, load manifest and parse.
-- Take id and use it to create the current library.
-- For all sources defined, call Macaroni parser to create model.
-- call default, which is defined to be "generate"

description="An example Test Project to be compiled by Macaroni."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost-filesystem", version="1.42.0"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
    print "A call was made to GENERATE!!!\n\n"
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"
    --run "JamGenerator"
    --local rtnCode = os.execute("bjam")
    --print("BJAM return code = " .. rtnCode .. ".")
    --if (rtnCode ~= 0) then
    --    error("Looks like the plan's not going swell.")
    --end
end

jamArgs = 
{ 	
	ExcludePattern = "Main.cpp .svn *Test.cpp",
	ExtraTargets = [[
	  	exe LuaTest 
	  		:	library
	  			../Source/Main.cpp	  			
	  		;
	  ]]
	};
		
function build()
	print("I is build method.\n");
	run("BoostBuild", jamArgs)
end

function install()
end


