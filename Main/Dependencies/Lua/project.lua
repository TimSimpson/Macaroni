require "os"
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
local Path = require "Macaroni.IO.Path";

--------------------------------------------------------------------------------
-- Plugins
--------------------------------------------------------------------------------
bjam = plugins:Get("BoostBuild2")
porg = plugins:Get("Porg")


--------------------------------------------------------------------------------
-- File Paths
--------------------------------------------------------------------------------
targetDirectory = "target"

--------------------------------------------------------------------------------
-- Project Model Information
--------------------------------------------------------------------------------
project = context:Group("Macaroni")
               :Project("Lua")
               :Version("5.2.3");
lib = project:Library{name="lib", shortName="LuaLib",
                      headers=pathList{}, sources=pathList{"Mh"},
                      dependencies={}}

--------------------------------------------------------------------------------
-- Actions
--------------------------------------------------------------------------------
generated = false
built = false
installed = false

function clean()
    local targetDir = Macaroni.IO.Path.New(targetDirectory)
    targetDir:ClearDirectoryContents();
end

function generate()
  if generated then return end

  local copyCFilesToCppFiles = function(src, dst)
    local srcFiles = src:GetPaths([[\.(c|h)?$]])
    for i = 1, #srcFiles do
      local fp = srcFiles[i];
      fp:CopyToDifferentRootPath(dst, true);
    end
    local dstFiles = dst:GetPaths([[\.(c)?$]])
    for j = 1, #dstFiles do
      local fp = dstFiles[j];
      local rp = fp.FileNameWithoutExtension .. ".cpp"
      --local rp = string.gsub(fp.RelativePath, ".c", ".cpp")
      fp:RenameRelative(rp)
    end
  end

  local getIncludePath = function()
    local success, path = pcall(function()
      return properties.lua["5.2.3"].include;
    end);
    if (not success) then
      error([[
  Could not find the variable properties.lua["5.2.3"].include.
  This can be set in the file "init.lua" located in the Macaroni directory under
  your home directory.
  The variable to set should look something like this:
  lua =
    {
      ["5.2.3"] =
      {
        bin = "C:/lua-5.2.3/bin",
        include = "C:/lua-5.2.3/src",
        source = "C:/lua-5.2.3/src"
      },
    },
      ]]);
    end
    return path;
  end

  local luaSource = getIncludePath()
  -- Copies all the lua source from some external directory defined in the
  -- properties file to the local CPP directory.
  -- In the process it renames the files to use the CPP extensions which
  -- makes it compile as C++ automatically in VC.
  local sourceDirectoryPath = Path.New(luaSource)
  local targetDirectoryPath = Path.New(targetDirectory)
  copyCFilesToCppFiles(sourceDirectoryPath, targetDirectoryPath);
  local originalLuaConf = targetDirectoryPath:NewPathForceSlash("luaconf.h")
  originalLuaConf:RenameRelative("luaconfOriginal.h")
  local newLuaConf = targetDirectoryPath:NewPathForceSlash("luaconf.h")
  local writer = newLuaConf:CreateFile()
  writer:WriteLine([[
#ifndef lconfig_h
#include "luaconfOriginal.h"
#undef LUA_IDSIZE
#define LUA_IDSIZE 512
#endif
    ]])
  writer:Close()


  local cppPath = Path.New(targetDirectory);
  -- Add extra library information. Because the files didn't exist we couldn't
  -- do this above, the way we normally would.
  -- Avoid the source files used as drivers.
  lib:Append{headers=pathList{targetDirectory},
             sources=pathList{targetDirectory},
             dependencies={},
             excludeFiles=pathList{
               targetDirectory .. "/lua.cpp",
               targetDirectory .. "/luac.cpp",
               --cppPath:NewPathForceSlash("lua.cpp"),
               --cppPath:NewPathForceSlash("luac.cpp"),
             }}
  lib:AddExe("lua", cppPath:NewPathForceSlash("lua.cpp"));
  --lib:AddExe("luac", cppPath:NewPathForceSlash("luac.cpp"));
  -- Now, extremely later than usual, porg is run.
  porg:Run("Generate", {target=lib})
  -- No need to run CPP as we aren't generating any.
  -- Generate Boost Build file.
  bjam:Run("Generate", { jamroot=cppPath:NewPath("/jamroot.jam"),
                         projectVersion=project,
                         output=output,
                         --flags={lib=[[
                         requirements=[[
                      <target-os>windows,<link>shared:<define>LUA_BUILD_AS_DLL=1
                         ]],
                         usageRequirements=[[
                      <target-os>windows,<link>shared:<define>LUA_BUILD_AS_DLL=1
                         ]],
  })

  generated = true
end

function build()
  if built then return end
  generate()
  os.execute("bjam " .. properties.bjam_options .. " target")
  built = true
end

function install()
    if installed then return end
    build()
    sinstall(project, filePath("./"))
    installed = true
end
