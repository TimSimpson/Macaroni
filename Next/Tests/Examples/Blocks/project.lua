import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="Macaroni.Examples",
  project="Blocks",
  version="1.0.0.0",
  libShortName = "ExampleBlocks",
  src="src",
  target="target",
  dependencies = {
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Boost-headers", "1.55"):Target("lib"),
  },
};
