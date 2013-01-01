import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. "NamespaceStatement",
  version=upper.Version,
  libShortName="MT_Namespace",
  tests={
     "Test.cpp"
  },
};
