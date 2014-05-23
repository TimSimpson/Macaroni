import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".Dependencies",
  version=upper.Version,
  docs="docs",
  src="src",
  target="target",
};
