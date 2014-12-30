import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

-- Added to init.lua:
boost_version = properties.boost.version

SimpleProject{
  group="Macaroni.Examples",
  project="TemplatesAndMacros",
  version="1.0.0.0",
  libShortName = "ExampleTemplatesAndMacros",
  src="src",
  target="target",
  dependencies = {
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Boost-headers", boost_version):Target("lib"),
  },
};
