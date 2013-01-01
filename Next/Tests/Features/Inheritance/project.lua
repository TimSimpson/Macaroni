import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".Inheritance",
  version=upper.Version,
  libShortName="MT_Inheritance",
  src="Source",
  target="GeneratedSource",
  tests={
     "Test.cpp"
  },
};
