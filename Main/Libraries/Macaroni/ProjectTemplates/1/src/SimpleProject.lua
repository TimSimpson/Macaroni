require "os"
require "Macaroni.IO.Path"
require "Macaroni.Model.Project.ExeTarget"

local ExeTarget = require "Macaroni.Model.Project.ExeTarget"
local Path = require "Macaroni.IO.Path"

local newPath = function(subPath)
    local p = Macaroni.IO.Path.New(getWorkingDirectory())
    return p:NewPathForceSlash(subPath)
end

local simpleProjectProperties = properties.simple_project or {}


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
        cpp:Run("Generate", { projectVersion=project, path=outputPath,
                              output=output })
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


-- Because nested Lua calls can get cut short, this function logs any errors
-- caused by func to the given output logger and then quits.
function runWithLoggedErrors(func)
    result, msg = pcall(func)
    if not result then
        output:ErrorLine(msg)
        error(msg)
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
    local html = nil;
    if args.htmlView then
        html = plugins:Get("HtmlView")
    end
    local bjam = plugins:Get("BoostBuild2")
    local cmake = plugins:Get("CMake")
    local rst = nil;
    if args.docs then
        rst = plugins:Get("Rst")
    end
    ---------------------------------------------------------------------------
    -- Helper functions.
    ---------------------------------------------------------------------------
    local newPath = function(subPath)
        local p = Path.New(getWorkingDirectory())
        return p:NewPathForceSlash(subPath)
    end

    ---------------------------------------------------------------------------
    -- Project Model Information
    ---------------------------------------------------------------------------
    local lProject = context:Group(args.group):Project(args.project)
                            :Version(args.version)
    local src = args.src or "src";
    local target = args.target or "target";
    local outputPath = filePath(target);
    local interfacePath = outputPath:NewPathForceSlash("lib.mh")

    -- Exclude the exe and test targets from the library.
    local excludePathStrings = {}
    function addRelativePathList(paths)
      for i,v in ipairs(paths) do
        excludePathStrings[#excludePathStrings + 1] = src .. "/" .. v
      end
    end

    -- If this is "loadingAsDependency" and an interface file exists (i.e. this
    -- project was already generated) we make the decision to nix any targets
    -- other than the main lib and in doing so save a ton of time.
    local lLibSources;
    if loadingAsDependency and interfacePath.Exists then
        lLibSources = pathList2{ interfacePath }
        -- A project loading this one won't need to know anything about it's
        -- exes or tests.
        args.exes = {}
        args.tests = {}
    else
        lLibSources = pathList{src}
    end

    addRelativePathList(args.exes)
    addRelativePathList(args.tests)
    local excludeFiles = pathList(excludePathStrings)

    local lLib = lProject:Library{
        name="lib",
        shortName = args.libShortName,
        headers=pathList{src, target},
        sources=lLibSources,
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

    local addTestDepsToExistingTargets = function()
        -- Add the extra test dependencies to all tests defined by this
        -- project.
        if MACARONI_VERSION == "0.2.3" then
            return
        end
        for i=1, #lProject.Targets do
            local target = lProject.Targets[i]
            if target.TypeName == "test" then
                for i, v in ipairs(args.testDependencies) do
                    local add = true
                    for j=1, #target.Dependencies do
                        if target.Dependencies[j] == v then
                            add = false;
                            break;
                        end
                    end
                    if add then
                        target:AddDependency(v)
                    end
                end
            end
        end
    end

    addTestDepsToExistingTargets()
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
    local generatedLess = false
    local generated = false
    local built = false
    local documented = false
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
        result, msg = pcall(function()
            porg:Run("Generate", {target=lLib})
            preGenerated = true
            end)
        if not result then
            output:ErrorLine(msg)
            error(msg)
        end

    end

    local cmakeFlags = args.cmakeFlags or {}
    cmakeFlags.projectVersion = lProject;
    cmakeFlags.filePath = outputPath
    cmakeFlags.output = output
    local lGenerateLess = function()
      if generatedLess then return end
      lPreGenerate()
      runWithLoggedErrors(function()
        cpp:Run("Generate", { projectVersion=lProject, path=outputPath,
                              output=output })
      end)
      generatedLess = true
    end

    local bjamFlags = args.bjamFlags or {}
    bjamFlags.jamroot = outputPath:NewPath("/jamroot.jam")
    bjamFlags.projectVersion = lProject;
    bjamFlags.output = output;

    local lGenerate = function()
      if generated then return end
      generateLess()

      runWithLoggedErrors(function()
        bjam:Run("Generate", bjamFlags)
      end)

      runWithLoggedErrors(function()
        cmake:Run("Generate", cmakeFlags);
      end)
      generated = true
    end

    local lBuild = function()
      if built then return end
      lGenerate()
      if (args.buildWithBoost ~= false) then
          bjam:Run("Build", bjamFlags)
      end
      if (cmakeFlags.invoke) then
          cmake:Run("Build", cmakeFlags);
      end
      built = true
    end

    local lDocument = function()
        if documented then return end
        lBuild()
        if args.htmlView then
            html:Run("Generate", { target=lLib, path=outputPath})
        end
        if args.docs then
            rst:Run("Build", {
                project=lProject,
                outputPath=targetDir,
                -- Important: the root path for every file parsed will be
                -- relative to the project's base, which is more useful for
                -- including files or snippets.
                sources={newPath(args.docs)},
                output=output
            })
            if simpleProjectProperties.run_sphinx then
                    output:DebugLine("'properties.simple_project.run_sphinx' "
                        .. "set to true, calling Sphinx...")
                rst:Run("Site", {
                    output=output,
                    outputPath=targetDir
                })
            else
                output:DebugLine("Call sphinx by setting "
                    .. "'properties.simple_project.run_sphinx' to true.")
            end

        end
    end

    local lInstall = function()
        if installed then return end
        lDocument()
        sinstall(lProject, filePath("./"))
        installed = true
    end

    generateLess = lGenerateLess
    generate = lGenerate
    build = lBuild
    document = lDocument
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
    local doct = false

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

    function document()
        if doct then return end
        for_each_child("document", false)
    end

    function install()
        if installed then return end
        for_each_child("install", false)
    end
end
