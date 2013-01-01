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
    loadBoostLib("headers"),
    load("Macaroni", "CppStd", "2003"):Target("lib")
  },
  exes={
     "Main.cpp"
  },
  tests={
     "Test.cpp"
  },
  testDependencies = {
    loadBoostLib("unit_test_framework"),
    loadBoostLib("test_exec_monitor"),
  },
};
