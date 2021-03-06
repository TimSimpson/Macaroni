require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".BoostBuild",
    version=upper.Version
}

description="A test to see if Boost Build can create and store C++ artifacts in a repo."

--source "Source"
--output = "GeneratedSource"

dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}

function generate()   
end

function build()   
end

function install()   
end

children =
{
	"SomeApp",
	"SomeLibA",
	"SomeLibB",
	
}



