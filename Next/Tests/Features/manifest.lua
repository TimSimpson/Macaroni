require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name=upper.Name .. ".Features",
    version=upper.Version
}

description="Battery of Tests for Macaroni."

output = "report"

children =
{
	"Annotations",
	"AdditionalAccessTypes",
	"BoostBuild",
	"ConstructorOverloading",
	"Destructors",
	"GlobalKeyword",
	"Inheritance",
	"LanguageFeatures",
	"Lua",
	"NamespaceStatement",
	--"OperatorOverloading",
};

dependency {group="Macaroni", name="Boost-filesystem", version="1.52"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.52"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
end

function build()
end

function install()
end


