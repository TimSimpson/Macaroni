require "os"
require "Macaroni.IO.Path"
require "Macaroni.Model.Project.ExeTarget"

local ExeTarget = Macaroni.Model.Project.ExeTarget

local newPath = function(subPath)
    local p = Macaroni.IO.Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end


--require "Plugin"

-- Useful for consuming a source directory and spitting out a directory of
-- C++ files.
-- Arguments:
--      group - Group of the project.
--      project - Project name.
--      version - Project version.
--      src     [ Optional ] - Location of Macaroni source code.
--      target  [ Optional ] - Directory to create C++ code.
--      dependencies [ Optional ] - Projects with Macaroni code to parse.

function GenerateCpp(args)
    args.dependencies = args.dependencies or {}
    local src = args.src or "src";
    local target = args.target or "target";

    local porg = plugins:Get("Porg")
    local cpp = plugins:Get("Cpp")

    project = context:Group(args.group):Project(args.project)
                            :Version(args.version)
    local lLib = project:Library{
        name="lib",
        headers=pathList{src, target},
        sources=pathList{src},
        dependencies = args.dependencies,
        excludeFiles = pathList({}),
        shortName = args.libShortName,
    }

    -- Instantly run this to generate physical objects in the model.
    porg:Run("Generate", {target=lLib})

    local targetDir = newPath(target)

    clean = function()
        targetDir:ClearDirectoryContents();
    end

    local generated = false

    generate = function()
      if generated then return end

      local outputPath = filePath(target)
      result, msg = pcall(function()
        cpp:Run("Generate", { projectVersion=project, path=outputPath })
        end)
      if not result then
        output:ErrorLine(msg)
        error(msg)
      end
      generated = true
    end

    build = function()
        generate()
    end

    install = function()
        build()
        sinstall(project, filePath("./"));
    end
end

-- Creates the project and lib vars, along with clean, generate, build, and
-- install functions.
function SimpleProject(args)
    ---------------------------------------------------------------------------
    -- Default arguments.
    ---------------------------------------------------------------------------
    args.exes = args.exes or {}
    args.tests = args.tests or {}
    args.testDependencies = args.testDependencies or {}

    ---------------------------------------------------------------------------
    -- Plugins
    ---------------------------------------------------------------------------
    local porg = plugins:Get("Porg")
    local cpp = plugins:Get("Cpp")
    local html = plugins:Get("HtmlView")
    local bjam = plugins:Get("BoostBuild2")

    ---------------------------------------------------------------------------
    -- Helper functions.
    ---------------------------------------------------------------------------
    local newPath = function(subPath)
        local p = Macaroni.IO.Path.New(getWorkingDirectory())
        return p:NewPathForceSlash(subPath)
    end

    ---------------------------------------------------------------------------
    -- Project Model Information
    ---------------------------------------------------------------------------
    local lProject = context:Group(args.group):Project(args.project)
                            :Version(args.version)
    local src = args.src or "src";
    local target = args.target or "target";

    -- Exclude the exe and test targets from the library.
    local excludePathStrings = {}
    function addRelativePathList(paths)
      for i,v in ipairs(paths) do
        excludePathStrings[#excludePathStrings + 1] = src .. "/" .. v
      end
    end
    addRelativePathList(args.exes)
    addRelativePathList(args.tests)
    local excludeFiles = pathList(excludePathStrings)

    local lLib = lProject:Library{
        name="lib",
        shortName = args.libShortName,
        headers=pathList{src, target},
        sources=pathList{src},
        dependencies = args.dependencies,
        excludeFiles = excludeFiles,
    }

    project = lProject
    lib = lLib

    -- Add the exe and test targets.
    local srcPath = newPath(src)
    function addExeTargets(paths)
      for i, v in ipairs(paths) do
        local realPath = srcPath:NewPathForceSlash(v)
        lLib.AddExe(lLib, realPath.FileNameWithoutExtension, realPath);
      end
    end
    addExeTargets(args.exes)

    local testCount = 1

    local createHeadersList = function(targets)
        local stringList = {}
        for i, v in ipairs(targets) do
            for j=1, #v do
                stringList[#stringList] = v[j]
            end
            end
        return pathList{stringList}
    end

    local addTestTarget = function(testPath)
        local realPath = srcPath:NewPathForceSlash(testPath)

        local deps = {}
        for i, v in ipairs(args.testDependencies) do
            deps[i] = args.testDependencies[i]
        end
        deps[#deps + 1] = lLib

        local realPath = srcPath:NewPathForceSlash(testPath)
        ExeTarget.Create{
            projectVersion=lProject,
            name=realPath.FileNameWithoutExtension,
            subType="test",
            dependencies=deps,
            sources=pathList2{realPath},
            };
    end

    local addTestTargets = function(testPaths)
        for i, testPath in ipairs(testPaths) do
            addTestTarget(testPath)
        end
    end

    addTestTargets(args.tests)

    -- This always runs, even if the user hasn't selected anything.
    -- It adds to the project model, but doesn't write any files.
    -- porg:Run("Generate", {target=lLib})
    -- TODO: Decide if this should be here or not. Some plugins may do things
    -- that must happen before the Physical model is expanded.


    ---------------------------------------------------------------------------
    -- Functions.
    ---------------------------------------------------------------------------
    local preGenerated = false
    local generated = false
    local built = false
    local installed = false

    local targetDir = newPath(target)
    -- Bind local vars in case the global changes.

    clean = function()
        targetDir:ClearDirectoryContents();
    end

    local lPreGenerate = function()
        if preGenerated then return end
        if args.preGenerate ~= nil then
            -- Allow client code to plug in additional behavior.
            local outputPath = filePath(target)
            args.preGenerate(outputPath)
        end
        porg:Run("Generate", {target=lLib})
        preGenerated = true
    end

    local lGenerate = function()
      if generated then return end
      lPreGenerate()
      local outputPath = filePath(target) --ath.New("target")
      result, msg = pcall(function()
        cpp:Run("Generate", { projectVersion=lProject, path=outputPath })
        end)
      if not result then
        output:ErrorLine(msg)
        error(msg)
      end
      html:Run("Generate", { target=lLib, path=outputPath})
      local bjamFlags = args.bjamFlags or {}
      bjamFlags.jamroot = outputPath:NewPath("/jamroot.jam")
      bjamFlags.projectVersion = lProject;
      bjamFlags.output = output;
      bjam:Run("Generate", bjamFlags)
      generated = true
    end

    local lBuild = function()
      if built then return end
      lGenerate()
      local cmd = "bjam " .. properties.bjam_options ..
                 " " .. targetDir.AbsolutePath .. " -d+2"
      output:DebugLine(cmd)
      if (os.execute(cmd) ~= 0) then
        output:ErrorLine("Failure running Boost Build!")
        error("Failure running Boost Build!")
      end
      built = true
    end

    local lInstall = function()
        if installed then return end
        lBuild()
        sinstall(lProject, filePath("./"))
        installed = true
    end

    generate = lGenerate
    build = lBuild
    install = lInstall
    project = lProject
    lib = lLib

end


function SimpleBoostProject(args)
    local newArgs = {}
    for k, v in pairs(args) do
        newArgs[k] = v
    end

    local createDepList = function(listA, listB)
        local newList = {}
        local addToList = function(otherList)
            if otherList then
                for i=1, #otherList do
                    newList[#newList + 1] = otherList[i]
                end
            end
        end
        addToList(listA);
        addToList(listB);
        return newList;
    end
    newArgs.dependencies = createDepList(
            {
                load("Macaroni", "CppStd", "2003"):Target("lib"),
                loadBoostLib("headers"),
            },
            args.dependencies
        );
    newArgs.testDependencies = createDepList(
            {
                loadBoostLib("unit_test_framework"),
                loadBoostLib("test_exec_monitor"),
            },
            args.testDependencies
        );
    return SimpleProject(newArgs);
end

function ParentProject(args)
    project = context:Group(args.group):Project(args.project)
                     :Version(args.version)
    local children = {}
    for i, v in ipairs(args.children) do
        children[#children + 1] = subProject(v)
    end

    local for_each_child = function(func_name, proceed_on_error)
        proceed_on_error = proceed_on_error or false
        for i, v in ipairs(children) do
            if not v:RunCommand(func_name .. "()", output) then
                if proceed_on_error then
                    output:ErrorLine("Error running child " .. tostring(v) .. ".")
                else
                    error("Error running child " .. tostring(v) .. ".")
                end
            end
        end
    end

    local preGenerated = false
    local generated = false
    local built = false
    local installed = false

    function clean()
        for_each_child("clean", true)
    end

    function generate()
        if generated then return end
        for_each_child("generate", false)
    end

    function build()
        if built then return end
        for_each_child("build", false)
    end

    function install()
        if installed then return end
        for_each_child("install", false)
    end
end
