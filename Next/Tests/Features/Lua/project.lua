import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

--MDOC-BEGIN pluginGet
local luaGlue = plugins:Get("LuaGlue");
--MDOC-END pluginGet

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".LuaGlue",
  version=upper.Version,
  libShortName = "LuaGlueTest",
  src="src",
  docs="docs",
  target="target",
  dependencies = {
    load("Macaroni", "Lua", "5.2.3"):Target("lib"),
  },
  exes={
    "Main.cpp"
  },
  tests={
     "Test.cpp"
  },
--MDOC-BEGIN preGenerate
  preGenerate = function(outputPath)
    luaGlue:Run("Generate", { target=lib, outputPath=outputPath,
                              luaImportCode=[[
#include <lauxlib.h>
#include <lualib.h>
                              ]]});
  end,
--MDOC-END preGenerate
};
