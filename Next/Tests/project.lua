import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

ParentProject{
  group=upper.Group,
  project="Macaroni.Tests",
  version=upper.Version,
  children={
    --"Bugs",
    "Examples",
    "Features",
  }
}
