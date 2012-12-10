import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

upper = getUpperProject();
print(upper)

local luaGlue = plugins:Get("LuaGlue");

SimpleProject{
  group=upper.Group,
  project=upper.Name .. ".LuaGlue",
  version=upper.Version,
  libShortName = "LuaGlueTest",
  src="Source",
  target="GeneratedSource",
  dependencies = {
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Lua", "5.1.4"):Target("lib"),
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
