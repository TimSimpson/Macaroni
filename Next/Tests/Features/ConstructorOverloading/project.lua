import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleProject{
  group=upper.Group,
  -- There's a bug in Boost Build (ok, not really, its my fault for abusing it)
  -- that precludes names of 114 chars or more. So... darn it! I'll need to
  -- add a repeatable hash or something instead.
  project=upper.Name .. ".LanguageFeatures.ConstructorOverloading",
  --project=upper.Name .. ".CtorOL",
  version=upper.Version,
  libShortName="CtorOverloading",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
  exes={
     "Main.cpp"
  },
  tests={
     "Test.cpp"
  },
};
