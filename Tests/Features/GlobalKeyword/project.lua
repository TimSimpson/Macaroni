import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleProject{
  group=upper.Group,
  project=upper.Name .. ".GlobalKeyword",
  version=upper.Version,
  libShortName="MT_GlobalKeyword",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
  tests={
     "Test.cpp"
  },
};