require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".Bugs",
    version= upper.Version -- "1.0.0.0"
}

description="Current and former known bugs experienced in Macaroni."

output = "target"

bugs = 
{
	"2",
	"1",		
};

function generate()
end

function build()
end

function install()
end


