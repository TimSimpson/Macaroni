require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. "NamespaceStatement",
    version=upper.Version,
}

description=[[Tests Macaroni's namespace statement.]]

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
	ExcludePattern = "Main.cpp Test.cpp .svn",
	Shared = True,
	Tests = {"Test.cpp"}
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs)	
end

