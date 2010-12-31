require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Tests - BoostBuild",
    version="1.0.0.0"
}

description="A test to see if Boost Build can create and store C++ artifacts in a repo."

--source "Source"
--output = "GeneratedSource"

dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}

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



