import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

ParentProject{
  group="Macaroni",
  project="Macaroni.Next",
  version="0.1.0.26",
  children={
    "Tests",
    "Release",
  }
}
