import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".OperatorOverloading",
  version=upper.Version,
  libShortName="MT_OperatorOverloading",
  src="Source",
  target="target",
  tests={
     "Test.cpp"
  },
};
