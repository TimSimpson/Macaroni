import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

--upper = getUpperProject();

ParentProject{
  group="Macaroni", -- upper.Group,
  project="Macaroni.Tests", --upper.Name .. ".AccessTypes",
  version="0.1.0.25", -- upper.Version,
  children={
    --"Bugs",
    "Examples",
    "Features",
  }
}
