import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

local luaGlue = plugins:Get("LuaGlue");

SimpleBoostProject{
  group=upper.Group,
  project=upper.Name .. ".LuaGlue",
  version=upper.Version,
  libShortName = "LuaGlueTest",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Lua", "5.2.3"):Target("lib"),
  },
  exes={
    "Main.cpp"
  },
  tests={
     "Test.cpp"
  },
  preGenerate = function(outputPath)
    luaGlue:Run("Generate", { target=lib, outputPath=outputPath,
                              luaImportCode=[[
#include <lauxlib.h>
#include <lualib.h>
                              ]]});
  end,
};
