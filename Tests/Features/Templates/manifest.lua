require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".Templates",
    version=upper.Version,
}

description=[[Tests Macaroni's limited ability to write templated classes.]]

sources = { "src" }
output = "target"

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group = "Macaroni", name = "Lua", version = "5.1.4" }

function generate()
    run("HtmlView");
    run("Cpp");    
end

jamArgs = 
{ 	
	ExcludePattern = "Main.cpp *Test*.cpp .svn",
	Shared = True,
	Tests = {"TestType.cpp", "TestNonType.cpp", "TestTemplate.cpp"}
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs)	
end

