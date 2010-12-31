require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.Tests",
    version=upper.Version
}

print "Greetings from the Test parent.";
description="Battery of Tests for Macaroni."

output = "target"

children = 
{
	"Bugs",	
	"Features",	
};

function generate()
end

function build()
end

function install()
end


