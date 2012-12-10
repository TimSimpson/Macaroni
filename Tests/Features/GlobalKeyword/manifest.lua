require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. "GlobalKeyword",
    version=upper.Version,
}

description='Tests the ~global keyword in Macaroni.'

sources = { "Source" }
output = "GeneratedSource"

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.52"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
    run("HtmlView");
    run("Cpp");
end

jamArgs =
{
	ExcludePattern = "Test.cpp .svn",
	ExtraTargets = [[	]],
	Shared = True,
	Tests = {"Test.cpp"}
};

function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs)
end

