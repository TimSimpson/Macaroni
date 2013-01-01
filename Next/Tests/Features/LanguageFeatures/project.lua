import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".LanguageFeatures",
  version=upper.Version,
  libShortName="MT_LanguageFeatures",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    loadBoostLib("filesystem"),
  },
  exes={
    "Main.cpp",
  },
  tests={
     "Macaroni/Tests/Blocks/BlocksTest.cpp",
     "Macaroni/Tests/Const/ConstTest.cpp",
     "Macaroni/Tests/Initializers/ConstructorInitializersTest.cpp",
     "Macaroni/Tests/Initializers/FieldInitializersTest.cpp",
     "Macaroni/Tests/Typedefs/TypedefsTest.cpp",
  },
};
