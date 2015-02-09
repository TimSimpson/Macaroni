import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();

ParentProject{
  group=upper.Group,
  project=upper.Name .. ".Features",
  version=upper.Version,
  children={
    "AccessTypes",
    "Annotations",
    "AutoDeclAndTrt",
    "ConstructorOverloading",
    "Dependencies",
    "Destructors",
    "Enum",
    "GlobalKeyword",
    "Inheritance",
    "LanguageFeatures",
    "Lua",
    "NamespaceStatement",
    --"OperatorOverloading",
    "PointerToPointers",
    "UnitTargets",
  }
}


-- project = context:Group("Macaroni")
--                  :Project("Macaroni.Tests.Features")
--                  :Version("0.1.0.24")

-- children = {}
-- function addChild(name)
--     children[#children + 1] = subProject(name)
-- end

-- addChild("AccessTypes")
-- addChild("Annotations")
-- addChild("ConstructorOverloading")
-- addChild("Destructors")
-- addChild("GlobalKeyword")
-- addChild("Inheritance")
-- addChild("LanguageFeatures")
-- addChild("Lua")
-- addChild("NamespaceStatement")
-- --addChild("OperatorOverloading")
-- addChild("UnitTargets")

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

