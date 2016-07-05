import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

ParentProject{
  group="Macaroni",
  project="Macaroni.Next",
  version="0.4.4",
  children={
    "Tests",
    "Release",
  }
}
