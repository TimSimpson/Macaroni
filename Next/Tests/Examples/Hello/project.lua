import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

-- Added to init.lua:
boost_version = properties.boost.version

SimpleProject{
  group="Macaroni.Examples",
  project="Hello",
  version="1.0.0.0",
  libShortName = "HelloApp",
  src="source",
  target="target",
  dependencies = {
    load("Macaroni", "Boost-headers", boost_version):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
  },
};
