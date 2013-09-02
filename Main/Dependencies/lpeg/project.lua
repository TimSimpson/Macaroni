require "os"
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

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
               :Project("lpeg")
               :Version("0.12");
lib = project:Library{name="lib", shortName="LPegLib",
                      headers=pathList{}, sources=pathList{"Mh"},
                      dependencies={
                        load("Macaroni", "Lua", "5.1.4"):Target("lib"),
                      }}

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
      return properties.lpeg["0.12"].include;
    end);
    if (not success) then
      error([[
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

  local luaSource = getIncludePath()
  copyCFilesToCppFiles(Macaroni.IO.Path.New(luaSource),
                       Macaroni.IO.Path.New(targetDirectory));
  local cppPath = Macaroni.IO.Path.New(targetDirectory);
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