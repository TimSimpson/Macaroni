import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

ParentProject{
  group="Macaroni",
  project="Macaroni.Next",
  version="0.3.0",
  children={
    "Tests",
    "Release",
  }
}
