import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".GlobalKeyword",
  version=upper.Version,
  libShortName="MT_GlobalKeyword",
  src="Source",
  target="GeneratedSource",
  tests={
     "Test.cpp"
  },
};
