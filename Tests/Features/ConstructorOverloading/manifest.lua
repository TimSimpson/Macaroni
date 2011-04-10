require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".LanguageFeatures.ConstructorOverloading",
    version=upper.Version
}

description="Proof of concept which overloads the Constructor of a class in various ways."

-- source "Source"
-- output = "GeneratedSource"

dependency {group="Macaroni", name="Boost-filesystem", version="1.46.1"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
    print "A call was made to GENERATE!!!\n\n"
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"
    
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
	run("BoostBuild", jamArgs)
end

function test()
	--run("BoostBuild", jamArgs);
end

function install()
end


