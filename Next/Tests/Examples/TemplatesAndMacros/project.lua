import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="Macaroni.Examples",
  project="TemplatesAndMacros",
  version="1.0.0.0",
  libShortName = "ExampleTemplatesAndMacros",
  src="src",
  target="target",
  dependencies = {
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Boost-headers", "1.55"):Target("lib"),
  },
};
