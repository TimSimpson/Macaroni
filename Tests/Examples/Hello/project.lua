import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

SimpleProject{
  group="Macaroni.Examples",
  project="Hello",
  version="1.0.0.0",
  libShortName = "AccessTypesApp",
  src="source",
  target="target",
  dependencies = {
    load("Macaroni", "CppStd", "2003"):Target("lib"),
  },
};
