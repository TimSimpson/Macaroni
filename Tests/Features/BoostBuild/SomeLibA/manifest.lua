require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. "SomeLibA",
    version=upper.Version,
}

description="A library."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name=upper.Name .. "SomeLibB", version=upper.Version}

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = ".svn *Test.cpp",	
	Shared = "true"
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs);
	run("InterfaceMh");	
end


