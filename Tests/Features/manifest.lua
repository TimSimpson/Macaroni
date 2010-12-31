require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Tests",
    version="1.0.0.0"
}

print "Greetings from the Test parent.";
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
    print "A call was made to PARENTAL GENERATE!!!\n\n"   
end

function build()
end

function install()
end


