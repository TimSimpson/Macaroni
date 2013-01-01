import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".Destructors",
  version=upper.Version,
  libShortName="MT_Dtors",
  src="Source",
  target="GeneratedSource",
  tests={
     "Test.cpp"
  },
};
