import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

ParentProject{
  group="Macaroni",
  project="Macaroni.Next",
  version="0.2.3",
  children={
    "Tests",
    "Release",
  }
}
