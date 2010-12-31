require "os"
require "Macaroni.Model.Library"

id =
{
    group="Macaroni",
    name="Macaroni.Tests",
    version="1.0.0.0"
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


