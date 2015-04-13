import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

Path = require "Macaroni.IO.Path"


SimpleBoostProject{
  group="Macaroni",
  project="Macaroni.Tests.Features.CppGenerator",
  version="DEV",
  src="src",
  --docs="docs",
  target="target",
};

local CodeGraph = require "Macaroni.Generators.Cpp.CodeGraph"

local graph = CodeGraph.Create()

local UnitFileGenerator = require "Macaroni.Generators.Cpp.Unit.UnitFileGenerator"
--project.
--local ug = UnitFileGenerator.Create()

local unit = project:GetTarget("Hats")
local ug = UnitFileGenerator.Create(unit)
--print("Unit generator = " .. ug)

local newPath = function(subPath)
    local p = Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end

local output = newPath("target")

ug:Generate(graph, output);
