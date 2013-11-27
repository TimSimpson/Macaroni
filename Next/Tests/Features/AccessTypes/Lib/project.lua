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
  project=upper.Name .. ".Lib",
  version=upper.Version,
  libShortName = "AccessTypesLib",
  src="Source",
  target="target",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.55"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
  tests={
     "Test.cpp",
     "Test2.cpp"
  },
  testDependencies = {
    load("Macaroni", "Boost-unit_test_framework", "1.55"):Target("lib"),
    load("Macaroni", "Boost-test_exec_monitor", "1.55"):Target("lib"),
  },
};
