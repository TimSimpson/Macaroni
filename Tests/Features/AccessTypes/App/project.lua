import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

description=[[
Macaroni generates implementation files which have their own distinct version
of the header.  This is so they can include private function symbols (and maybe
other symbols if this idea turns out not be terrible) and make the publicly
exposed headers simpler.
]]

SimpleProject{
  group=upper.Group,
  project=upper.Name .. ".App",
  version=upper.Version,
  libShortName = "AccessTypesApp",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.51"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load(upper.Group, upper.Name .. ".Lib", upper.Version):Target("lib"),
  },
  tests={
     "Test.cpp"
  },
};
