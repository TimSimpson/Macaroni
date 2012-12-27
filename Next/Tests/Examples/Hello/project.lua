import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="Macaroni.Examples",
  project="Hello",
  version="1.0.0.0",
  libShortName = "HelloApp",
  src="source",
  target="target",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
  },
};
