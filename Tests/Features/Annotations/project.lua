import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleProject{
  group=upper.Group,
  project=upper.Name .. ".Annotations",
  version=upper.Version,
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.46.1"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
};
