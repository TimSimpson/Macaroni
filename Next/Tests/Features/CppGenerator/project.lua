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
local UnitFileGenerator = require "Macaroni.Generators.Cpp.Unit.UnitFileGenerator"

local newPath = function(subPath)
    local p = Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end



local graph = CodeGraph.Create()
local unit = project:GetTarget("Hats")
local ug = UnitFileGenerator.Create(unit)
--print("Unit generator = " .. ug)


local output = newPath("target")

ug:Generate(graph, "LIBDECL_THING", output);
