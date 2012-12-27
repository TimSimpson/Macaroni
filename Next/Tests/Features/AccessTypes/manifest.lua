require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".AdditionalAccessTypes",
    version=upper.Version
}

description=[[
Macaroni adds two extra access types, ~internal and ~hidden.  This test confirms
they work as expected.
]]


function generate()
end

function build()
end

function install()
end

children =
{
	"Lib",
	"App",
}



