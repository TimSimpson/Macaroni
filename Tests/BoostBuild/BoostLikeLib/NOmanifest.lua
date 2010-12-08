require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Tests - BoostBuild - BoostLikeLib",
    version="1.0.0.0"
}

description="Attempting to create a library using boost config.";

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="CppStd", version="2003"}
--dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = ".svn *Test.cpp",	
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("InterfaceMh");
	run("BoostBuild", jamArgs);	
end


