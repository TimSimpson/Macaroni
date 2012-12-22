require "os"
require "Macaroni.IO.Path"
--upper = getUpperProject();

project = context
    :Group("Macaroni")
    :Project("Macaroni.Main")
    :Version("0.1.0.26")


---------------------------------------------------------------------------
-- Helper functions.
--------------------------------------------------------------------------
local newPath = function(subPath)
    local p = Macaroni.IO.Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end

---------------------------------------------------------------------------
-- Plugins
---------------------------------------------------------------------------
local porg = plugins:Get("Porg")
local cpp = plugins:Get("Cpp")
local html = plugins:Get("HtmlView")
local bjam = plugins:Get("BoostBuild2")


---------------------------------------------------------------------------
-- Dependencies
---------------------------------------------------------------------------
local core_dependencies={
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "Boost-filesystem", "1.52"):Target("lib"),
    --load("Macaroni", "Boost-regex", "1.52"):Target("lib"),
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    load("Macaroni", "Lua", "5.1.4"):Target("lib"),
};


---------------------------------------------------------------------------
-- Project Model Information
---------------------------------------------------------------------------
local src = "Source/main/mcpp"
local target = "GeneratedSource"

local excludeFiles = pathList({
    src .. "/Main.cpp"
});

local lib = project:Library{
    name="lib",
    shortName = "macaroni-core",
    headers=pathList{src, target},
    sources=pathList{src},
    dependencies = dependencies,
    excludeFiles = excludeFiles,
}

-- Add the exe and test targets.
local exes = {};
local tests = {};

local srcPath = newPath(src)
function addTargets(paths, func)
  for i, v in ipairs(paths) do
    local realPath = srcPath:NewPathForceSlash(v)
    lib[func](lLib, realPath.FileNameWithoutExtension, realPath);
  end
end
addTargets(exes, "AddExe")
addTargets(tests, "AddTest")


---------------------------------------------------------------------------
-- Functions.
---------------------------------------------------------------------------
local preGenerated = false
local generated = false
local built = false
local installed = false

local targetDir = newPath(target)


clean = function()
    targetDir:ClearDirectoryContents();
end


generate = function()
  if generated then return end

  porg:Run("Generate", {target=lib})

  local outputPath = filePath(target)
  result, msg = pcall(function()
    cpp:Run("Generate", { projectVersion=project, path=outputPath })
    end)
  if not result then
    output:ErrorLine(msg)
    error(msg)
  end
  html:Run("Generate", { target=lib, path=outputPath})
  local bjamFlags = args.bjamFlags or {}
  bjamFlags.jamroot = outputPath:NewPath("/jamroot.jam")
  bjamFlags.projectVersion = project;
  bjamFlags.output = output;
  bjam:Run("Generate", bjamFlags)
  generated = true
end

