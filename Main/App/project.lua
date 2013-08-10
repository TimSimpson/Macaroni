require "os"
require "Macaroni.IO.Path"


project = context
    :Group("Macaroni")
    :Project("Macaroni.App")
    :Version("0.1.0.29")


---------------------------------------------------------------------------
-- Helper functions.
--------------------------------------------------------------------------
local newPath = function(subPath)
    local p = Macaroni.IO.Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end

local newRootPath = function(subPath)
    return newPath(subPath):CreateWithCurrentAsRoot()
end

local stopWatch = function(label)
    local startTime = os.time()
    return function()
        local endTime = os.time()
        output:WriteLine("                                                 "
              .. label .. " " .. tostring(endTime - startTime));
        return endTime - startTime
    end
end

local copyFiles = function(src, dst, regex)
    local filePaths = src:GetPaths(regex)
    for i = 1, #filePaths do
        local fp = filePaths[i];
        fp:CopyToDifferentRootPath(dst, true);
        -- print(tostring(fp));
    end
end

local copyCppFiles = function(src, dst)
    copyFiles(src, dst, [[\.(c|cpp|h|hpp)?$]]);
end

function copyJamFiles(src, dst)
    copyFiles(src, dst, [[\.jam?$]]);
end

function copyResourceFiles(src, dst)
    copyFiles(src, dst, [[\.(ico|rc|rc2|h)?$]]);
end


---------------------------------------------------------------------------
-- Plugins
---------------------------------------------------------------------------
local luaGlue = plugins:Get("LuaGlue")
local porg = plugins:Get("Porg")
local cpp = plugins:Get("Cpp")
local html = plugins:Get("HtmlView")
local bjam = plugins:Get("BoostBuild2")
local replCommand = plugins:Get("Source/main/lua/ReplCommand")
local versionInfo = plugins:Get("Source/main/lua/VersionInfoGenerator")


---------------------------------------------------------------------------
-- Dependencies
---------------------------------------------------------------------------
local core_dependencies={
    load("Macaroni", "Boost-headers", "1.52"):Target("lib"),
    load("Macaroni", "Boost-filesystem", "1.52"):Target("lib"),
    load("Macaroni", "Boost-regex", "1.52"):Target("lib"),
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
    dependencies = core_dependencies,
    excludeFiles = excludeFiles,
}

-- Add the exe and test targets.
local exes = { };
local tests = {};

local srcPath = newPath(src)
function addTargets(paths, func)
    for i, v in ipairs(paths) do
        local realPath = srcPath:NewPathForceSlash(v)
        lib[func](lib, realPath.FileNameWithoutExtension, realPath);
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

generated = false
built = false

clean = function()
    targetDir:ClearDirectoryContents();
    local pureCppDir = newPath("PureCpp");
    pureCppDir:ClearDirectoryContents();
end

local clockRun = function(label, f)
    local watch = stopWatch(label)
    f()
    watch()
end

  -- generateLua(filePath(target))
  -- porg:Run("Generate", {target=lib});
  -- for i=1, #project.Targets do
  --     print(project.Targets[i])
  -- end

generate = function()
    local outputPath = filePath(target);

    if generated then return end

    local generateLua = function()
        luaGlue:Run("Generate", {
            target=lib,
            outputPath=outputPath,
            luaImportCode=[[
    #include <lauxlib.h>
    #include <lualib.h>
            ]]
            });
    end

    local generateCpp = function()
        porg:Run("Generate", {target=lib});

        result, msg = pcall(function()
            cpp:Run("Generate", { projectVersion=project, path=outputPath})
            end)
        if not result then
            output:ErrorLine(msg)
            error(msg)
        end
    end

    local generateHtml = function()
        html:Run("Generate", { target=lib, path=outputPath})
    end

    local generateBjam = function()
        local bjamFlags = {
            jamroot = outputPath:NewPath("/jamroot.jam"),
            projectVersion = project,
            output = output,
        }
        bjam:Run("Generate", bjamFlags)
    end

    local generateVersionInfo = function()
        versionInfo:Run("Generate", { projectVersion=project, path=outputPath })
    end

    local generateReplCommands = function()
      replCommand:Run("Generate", { context=context })
    end

    local runAll = function()
        local watch = stopWatch("GENERATE")
        clockRun("REPL", generateReplCommands)
        clockRun("LUA", generateLua)
        --clockRun("HTML", generateHtml)
        clockRun("C++", generateCpp)
        clockRun("BOOST BUILD", generateBjam)
        clockRun("VERSION INFO", generateVersionInfo)
        generated = true
        watch()
    end

    runAll()
end


build = function()
  local callBjam = function()
      local cmd = "bjam link=static threading=multi " -- cxxflags=-std=gnu++11 "
                  .. properties.bjam_options .. " " .. targetDir.AbsolutePath
      output:WriteLine(cmd)
      if (os.execute(cmd) ~= 0) then
          output:ErrorLine("Failure running Boost Build!")
          output:ErrorLine(cmd)
          error("Failure running Boost Build!")
      end
  end

  local createPureCpp = function()
      -- Creates a directory with the pure C++ source.
      local dstDir = newRootPath("PureCpp");
      copyCppFiles(newRootPath("../Dependencies/Lua/target"), dstDir);
      copyCppFiles(newRootPath("Source/main/mcpp"), dstDir);
      copyResourceFiles(newRootPath("Source/main/resources"), dstDir);
      copyCppFiles(newRootPath("GeneratedSource"), dstDir);
      copyFiles(newRootPath("Source/main/pureCppExtraFiles"), dstDir,
                [[\.(jam|txt)?$]]);
      -- copy license
      newRootPath("GeneratedSource"):NewPathForceSlash("LICENSE.txt")
        :CopyToDifferentRootPath(dstDir, true);
  end

  local runTests = function()
      output:WriteLine([[
********************************************************************************
 Running Unit tests...
********************************************************************************
]])
      local exePath = targetDir:NewPathForceSlash("exe/macaroni.exe")
      local testDir = newPath(src):NewPathForceSlash("../../test/lua")
      local cmd = exePath.AbsolutePath .. " luaTests "
                  .. testDir.AbsolutePath
      output:WriteLine(cmd)
      if (os.execute(cmd) ~= 0) then
          output:ErrorLine("Failing running tests!")
          error("Failure running tests.")
      end
  end

  if not built then
      generate()
      callBjam()
      runTests()
      createPureCpp()
      built = true
  end
end

