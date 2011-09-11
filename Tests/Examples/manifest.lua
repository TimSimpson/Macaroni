require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name=upper.Name .. ".Examples",
    version=upper.Version
}

description="Examples for Macaroni."

output = "target"

children = 
{
	"Hello"
};

function generate()
end

function build()
end

function install()
end


