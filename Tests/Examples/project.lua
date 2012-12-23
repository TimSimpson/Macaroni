import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

ParentProject{
  group=upper.Group,
  project=upper.Name .. ".Examples",
  version=upper.Version,
  children={
    --"Bugs",
    "Blocks",
    "Hello",
  }
}
