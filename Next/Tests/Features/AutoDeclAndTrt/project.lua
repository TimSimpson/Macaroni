import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

-- Added to init.lua:
boost_version = properties.boost.version

SimpleProject{
  group=upper.Group,
  project=upper.Name .. ".AutoDeclAndTrt",
  version=upper.Version,
  libShortName="AutoDeclAndTrt",
  docs="docs",
  dependencies = {
    load("Macaroni", "Boost-headers", boost_version):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
};
