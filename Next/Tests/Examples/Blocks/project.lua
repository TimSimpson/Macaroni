import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

-- Added to init.lua:
boost_version = properties.boost.version

SimpleProject{
  group="Macaroni.Examples",
  project="Blocks",
  version="1.0.0.0",
  libShortName = "ExampleBlocks",
  src="src",
  docs="docs",
  target="target",
  dependencies = {
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Boost-headers", boost_version):Target("lib"),
  },
};
