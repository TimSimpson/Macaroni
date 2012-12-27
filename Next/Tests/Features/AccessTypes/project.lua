import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

-- project = context:Group(upper.Group)
--                  :Project(upper.Name .. ".AdditionalAccessTypes")
--                  :Version(upper.Version)
-- description=[[
-- Macaroni adds two extra access types, ~internal and ~hidden.  This test confirms
-- they work as expected.
-- ]]

-- children = {}
-- function addChild(name)
--     children[#children + 1] = subProject(name)
-- end

-- addChild("Lib")
-- addChild("App")

-- function clean()
--     for i, v in ipairs(children) do
--         v:RunCommand("clean()", output)
--     end
-- end

-- function build()
--     for i, v in ipairs(children) do
--         v:RunCommand("build()", output)
--     end
-- end

ParentProject{
  group=upper.Group,
  project=upper.Name .. ".AccessTypes",
  version=upper.Version,
  children={
    "Lib",
    "App"
  }
}
