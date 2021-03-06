require "os"
require "Macaroni.IO.GeneratedFileWriter";
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
               :Project("MoonScriptDeps")
               :Version("DEV");
lib = project:Library{name="lib", shortName="MoonScriptDeps",
                      headers=pathList{}, sources=pathList{"Mh", "target/lua"},
                      dependencies={
                        load("Macaroni", "Lua", "5.2.3"):Target("lib"),
                      }}




--------------------------------------------------------------------------------
-- Actions
--------------------------------------------------------------------------------
generated = false
built = false
installed = false

function clean()
    local targetDir = Path.New(targetDirectory)
    targetDir:ClearDirectoryContents();
end

function generate()
  if generated then return end

  local copyCFilesToCppFiles = function(src, dst, extension)
    local srcFiles = src:GetPaths([[\.(c|h)?$]])
    for i = 1, #srcFiles do
      local fp = srcFiles[i];
      fp:CopyToDifferentRootPath(dst, true);
    end
    local dstFiles = dst:GetPaths([[\.(c)?$]])
    for j = 1, #dstFiles do
      local fp = dstFiles[j];
      local rp = fp.FileNameWithoutExtension .. extension
      fp:RenameRelative(rp)
    end
  end

  local crash = function(msg)
      output:ErrorLine(msg)
      error("Couldn't find dependency.")
  end

  local getLpegIncludePath = function()
    local success, path = pcall(function()
      return properties.lpeg["0.12"].source;
    end);
    if (not success) then
      crash([[
  Could not find the variable properties.lpeg["0.12"].include.
  This can be set in the file "init.lua" located in the Macaroni directory under
  your home directory.
  The variable to set should look something like this:

  lpeg =
    {
      ["0.12"] =
      {
        bin = "C:/lpeg-0.12/bin",
        include = "C:/lpeg-0.12/src",
        source = "C:/lpeg-0.12/src"
      },
    },
      ]]);
    end
    return path;
  end

  local getLuaFileSystemSource = function()
    local success, path = pcall(function()
        return properties.luafilesystem["1.6.2"].source;
      end);
      if (not success) then
        crash([[
    Could not find the variable properties.lpeg["0.12"].include.
    This can be set in the file "init.lua" located in the Macaroni directory under
    your home directory.
    The variable to set should look something like this:

    luafilesystem =
      {
        ["1.6.2"] =
        {
          source = "C:/luafilesystem/src",
        },
      },
        ]]);
      end
      return path;
  end


  local lpegSource = getLpegIncludePath()
  copyCFilesToCppFiles(Path.New(lpegSource),
                       Path.New(targetDirectory),
                       ".cpp");

  -- TODO:
  -- local luaFileSystemSource = getLuaFileSystemSource()
  -- copyCFilesToCppFiles(Path.New(luaFileSystemSource),
  --                      Path.New(targetDirectory),
  --                      ".cpp");

  local cppPath = Path.New(targetDirectory);
  lib:Append{headers=pathList{targetDirectory},
             sources=pathList{targetDirectory},
             dependencies={},
             excludeFiles=pathList{
             }}
  porg:Run("Generate", {target=lib})
  bjam:Run("Generate", { jamroot=cppPath:NewPath("/jamroot.jam"),
                         projectVersion=project,
                         output=output,
                         --flags={lib=[[
                         requirements=[[
                      <target-os>windows<link>shared:<define>LUA_BUILD_AS_DLL=1
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
