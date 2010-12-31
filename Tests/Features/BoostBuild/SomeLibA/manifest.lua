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

dependency {group="Macaroni", name="Tests - BoostBuild - SomeLib B", version="1.0.0.0"}

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
	run("BoostBuild", jamArgs);
	
end


