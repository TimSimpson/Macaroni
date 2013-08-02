import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".DefaultArguments",
  version=upper.Version,
  libShortName="MT_DefaultArgs",
  src="src",
  target="target",
  tests={
     "Test.cpp"
  },
};
