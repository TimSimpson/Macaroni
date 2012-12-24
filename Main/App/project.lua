require "os"
require "Macaroni.IO.Path"


project = context
    :Group("Macaroni")
    :Project("Macaroni.App")
    :Version("0.1.0.26")


---------------------------------------------------------------------------
-- Helper functions.
--------------------------------------------------------------------------
local newPath = function(subPath)
    local p = Macaroni.IO.Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
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
      local cmd = "bjam -d+2 -j8 "
                  .. targetDir.AbsolutePath
                  .. " link=static"
      output:WriteLine(cmd)
      if (os.execute(cmd) ~= 0) then
          output:ErrorLine("Failure running Boost Build!")
          output:ErrorLine(cmd)
          error("Failure running Boost Build!")
      end
  end
  if not built then
      generate()
      callBjam()
      built = true
  end
end
