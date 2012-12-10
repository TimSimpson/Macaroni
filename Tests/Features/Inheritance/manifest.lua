require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. "Inheritance",
    version=upper.Version,
}

description=[[
Tests the Inheritance functionality of the parser, model, and C++ code
generator.
]]

sources = { "Source" }
output = "Target"

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.52"}
dependency {group="Macaroni", name="CppStd", version="2003"}

function generate()
    run("HtmlView");
    run("Cpp");
    run("InterfaceMh");
end

jamArgs =
{
	ExcludePattern = "Main.cpp Test.cpp .svn",
	Shared = True,
	Tests = {"Test.cpp"}
};

function build()
	run("BoostBuild", jamArgs)
end
