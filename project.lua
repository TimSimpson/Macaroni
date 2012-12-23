import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

--upper = getUpperProject();

ParentProject{
  group="Macaroni", -- upper.Group,
  project="Macaroni.All", --upper.Name .. ".AccessTypes",
  version="0.1.0.26", -- upper.Version,
  children={
    --"Bugs",
    --"Examples",
    "Tests",
    "Release",
  }
}
