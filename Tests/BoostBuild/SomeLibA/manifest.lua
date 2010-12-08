require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Tests - BoostBuild - SomeLib A",
    version="1.0.0.0"
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


