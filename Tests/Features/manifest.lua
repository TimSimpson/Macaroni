require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Macaroni.Tests.Features",
    version="1.0.0.0"
}

description="Battery of Tests for Macaroni."

output = "report"

children = 
{
	"Attributes",
	"BoostBuild",	
	"LanguageFeatures",
	"Lua"
};

dependency {group="Macaroni", name="Boost-filesystem", version="1.42.0"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
end

function build()
end

function install()
end


