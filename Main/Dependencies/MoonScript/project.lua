require "os"
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

Files = Macaroni.Model.Project.Files
FileSet = Macaroni.IO.FileSet;



--------------------------------------------------------------------------------
-- File Paths
--------------------------------------------------------------------------------
sourceDirectory = "src"
targetDirectory = "target"

--------------------------------------------------------------------------------
-- Project Model Information
--------------------------------------------------------------------------------
project = context:Group("Macaroni")
               :Project("MoonScript")
               :Version("DEV");
project:Resource{name = "scripts", type="lua", files = pathList{"target/lua"}}



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

  local crash = function(msg)
      output:ErrorLine(msg)
      error("Couldn't find dependency.")
  end

  local copyLuaFiles = function(src, dst, extension)
    if dst == nil then
        error("Argument 2 cannot be nil.", 2)
    end
    local srcFiles = src:GetPaths([[\.(lua|moon)?$]])
    for i = 1, #srcFiles do
      local fp = srcFiles[i];
      fp:CopyToDifferentRootPath(dst, true);
    end
  end

  local getMoonScriptSource = function()
    local success, path = pcall(function()
        return properties.moonscript.root;
      end);
      if (not success) then
        crash([[
    Could not find the variable properties.moonscript.root.
    This can be set in the file "init.lua" located in the Macaroni directory under
    your home directory.
    The variable needs to contain the base directory of moonscript's repo
    and should look something like this:

    moonscript =
      {
        "root" = "C:/moonscript"
      },
        ]]);
      end
      return path;
  end

  local srcRoot = Macaroni.IO.Path.New(getMoonScriptSource())
  local outputRoot = Macaroni.IO.Path.New(targetDirectory)
                     :NewPathForceSlash("lua")

  local source1 = srcRoot:NewPathForceSlash("moonscript")
  copyLuaFiles(source1, outputRoot)

  local source2 = srcRoot:NewPathForceSlash("moon")
  copyLuaFiles(source2, outputRoot)

  local moonLua = srcRoot:NewPathForceSlash("moon.lua")
  moonLua:CopyToDifferentRootPath(outputRoot, true);
  local moonscriptLua = srcRoot:NewPathForceSlash("moonscript.lua")
  moonscriptLua:CopyToDifferentRootPath(outputRoot, true);

  local helperScript = Macaroni.IO.Path.New(sourceDirectory)
                       :NewPathForceSlash("MacaroniMoonScriptLoader.lua")
  helperScript:CopyToDifferentRootPath(outputRoot, true);

  generated = true
end

function build()
  generate()
end

function install()
    if installed then return end
    build()
    sinstall(project, filePath("./"))
    installed = true
end
