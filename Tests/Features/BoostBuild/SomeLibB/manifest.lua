require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name= upper.Name .. "SomeLibB",
    version=upper.Version
}

description="A library."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = ".svn *Test.cpp"
};
		
function build()
	startTime = os.clock()
	run("BoostBuild", jamArgs)
	endTime = os.clock()
	print("Build time = " .. tostring(endTime - startTime))
end

function install()
	run("InterfaceMh");
	run("BoostBuild", jamArgs);	
end


