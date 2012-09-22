require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".App",
    version=upper.Version
}

description=[[
Macaroni generates implementation files which have their own distinct version
of the header.  This is so they can include private function symbols (and maybe
other symbols if this idea turns out not be terrible) and make the publicly
exposed headers simpler.
]]

dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {
    group=upper.Group,
    name=upper.Name .. ".Lib",
    version=upper.Version
}

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"
end

function build_and_install()
	run("BoostBuild", {
		ExcludePattern = "Main.cpp .svn *Test*.cpp",
		Shared= True,
		Tests = {"Test.cpp"}
	})
end

build = build_and_install
install = build_and_install



