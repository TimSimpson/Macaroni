import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

ParentProject{
  group="Macaroni",
  project="Macaroni.Main",
  version=upper.Version,
  children={
   -- "App",
  }
}
