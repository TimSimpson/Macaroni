require "string"
local Hash = require "Macaroni.Core.Hash";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.IO.Path";
require "Log"
require "Cpp/LibraryConfigGenerator";
require "Plugin"

Target = require "Macaroni.Model.Project.Target";

--require "Macaroni.IO.PathList";


function debugLine(self, msg)
    if MACARONI_VERSION == "0.1.0.27" then
        self.output:WriteLine(msg);
    else
        self.output:DebugLine(msg);
    end
end

function GetMethod(name)
    if name == "Generate" then
        return
        {
            Describe = function(args)
                validateArgs(args);
                args.output.WriteLine("Create Boost Build file for project "
                                      .. tostring(args.projectVersion)
                                      .. "to the directory "
                                      .. tostring(args.path) .. ".");
            end,
            Run = function(args)
                validateArgs(args);
                Initialize(args);
                Generate(args)
            end
        }
    elseif name == "Build" then
        return
        {
            Describe = function(args)
                validateArgs(args);
                args.output.WriteLine("Call Boost Build.");
            end,
            Run = function(args)
                validateArgs(args);
                Initialize(args);
                Build(args)
            end
        }
    elseif name == "Install" then
        return
        {
            Describe = function(args)
                validateArgs(args);
                args.output.WriteLine("Create Boost Build file for project "
                                      .. tostring(args.projectVersion)
                                      .. "to the directory "
                                      .. tostring(args.path) .. ".");
            end,
            Run = function(args)
                local path = findOrCreateInstallPath(args.projectVersion)
                --TODO: Make the h files get included, somehow.
                -- This next part copies all C++ files.
                local self = args;
                local dst = path:NewPathForceSlash("Source")
                args.output:DebugLine("INSTALLING TO PATH " .. tostring(dst))
                for target in Plugin.IterateProjectVersionTargets(
                    self.projectVersion, "unit")
                do
                    local cppFile = target.CppFile
                    local hFile = target.HFile
                    args.output:DebugLine(tostring(target))
                    args.output:DebugLine("\t" .. tostring(cppFile))
                    args.output:DebugLine("\t" .. tostring(hFile))
                    cppFile:CopyToDifferentRootPath(dst)
                    if hFile ~= nil then
                        hFile:CopyToDifferentRootPath(dst)
                    end
                end
                --TODO: Create a jamroot.jam file in the installation directory
                -- using only these targets. Pass in the install location into
                -- the existing jamroot.jam creation functions and have them
                -- modify any path they write out to match this.
            end
        }
    end
    return nil;
end




-------------------------------------------------------------------------------
-- Create jamroot.jam at args.jamrootPath.
--
-------------------------------------------------------------------------------

function Initialize(self)
    if self.defaultLib == nil then
        for i=1, #self.projectVersion.Targets do
            local target = self.projectVersion.Targets[i]
            if target.Name == "lib" then
                self.defaultLib = target
            end
        end
    end
    self.libTargetDeps = {}
    self.flags = self.flags or {}
end

function Generate(self)
    validateArgs(self)
    writeBoostFile(self)
end

function Build(self)
    local targetDir = self.jamroot.ParentPath.AbsolutePath
    local b2name = properties.b2_name or "b2"
    local cmd = "bjam " .. properties.bjam_options ..
                     " " .. targetDir
    for i,v in ipairs(properties.buildOptions) do
        cmd = cmd .. " " .. v
    end
    self.output:DebugLine(cmd)
    local success, exit, number = os.execute(cmd)
    if (not success or exit ~= "exit" or number ~= 0) then
        self.output:ErrorLine("Failure running Boost Build! ")
        error("Failure running Boost Build!")
    end
end

function validateArgs(self)
    Plugin.Check(self.projectVersion ~= nil, "Missing argument 'projectVersion'.")
    Plugin.Check(self.jamroot ~= nil, "Missing argument 'jamroot'.")
    Plugin.Check(self.output ~= nil, "Missing argument 'output'.")
    --self.Log = self.output -- Log or Plugin.CreateFakeLog()
end

function getProjectName(projectVersion)
    return "MACARONI_PROJECT_" .. projectVersion:GetCId();
end


function getDepName(self, target)
    local prefix = ""

    function getDepUnitName(target)
        if target.CppFile ~= nil then
            return Hash.GenerateHash(prefix .. "MACARONI_UNIT_TARGET_"
                .. target:GetCId());
        else
            return nil
        end
    end

    function getDepLibName(target)
        if isBoostDepProject(target.ProjectVersion) then
            -- This is a hideous hack.
            --TODO: Get some kind of annotation on libraries to make this cleaner.
            --      In general, this would become some custom Boost Build behavior.
            local boostSuffix = string.sub(target.ProjectVersion.Name, 7)
            return "/boost//" .. boostSuffix ;
        end
        if target.Name == "lib" then
            local name = prefix .. "MLT_" .. target:GetCId();
            if string.len(name) > 113 then
                local msg = "BAD"
                self.output:WriteLine("WARNING: Target name " .. name ..
                    " exceeds Boost Build's limit for target names by roughly " ..
                    tostring(string.len(name) - 113) .. " characters!")
            end
            return name
        else
            return target.GetShortCId();
        end
    end


    if target.ProjectVersion:GetCId() ~= self.projectVersion:GetCId() then
        prefix = "/" .. getProjectName(target.ProjectVersion) .. "//"
    end
    -- Boost Build has about a roughly 114 character limit on target names.
    if target.ShortName ~= nil then
        return prefix .. target.ShortName
    end
    if target.TypeName == "unit" then
        return getDepUnitName(target)
    end
    if target.TypeName == "lib" then
        return getDepLibName(target)
    end
    if target.TypeName == "exe" then
        --return prefix .. "MACARONI_EXE_TARGET_" .. target:GetShortCId();
        local exeName = target.ShortName or target.Name;
        return prefix .. exeName;
    end
    if target.TypeName == "test" then
        return prefix .. "MACARONI_TEST_TARGET_" .. target:GetShortCId();
    end
    error("Can't create dep name for " .. tostring(target) .. "!")
end

function allDependencies(self, target)
    -- Returns a string with all a targets deps seperated by spaces.
    local t = {};
    for target in Plugin.IterateDependencies(target) do
        local dep = getDepName(self, target);
        if dep ~= nil then
            t[#t + 1] = dep
        end
    end
    return table.concat(t, "\n        ");
end

function allDependenciesWithJamSupport(self, target)
    -- Returns a string with all a targets with jam support seperated by spaces
    local t = {};
    local myPid = target.ProjectVersion:GetCId();
    for target in Plugin.IterateDependencies(target) do
        local pid = target.ProjectVersion:GetCId();
        if target.TypeName == 'unit' or
           self.jamSupport[pid] or pid == myPid then
            local dep = getDepName(self, target);
            if dep ~= nil then
                t[#t + 1] = dep
            end
        else
            t[#t + 1] = "# NO DEP FOR YOU! " .. tostring(target)
        end
    end
    return table.concat(t, "\n        ");
end

function allChildTargets(self, libTarget)
    -- Returns a string with all child targets of a lib seperated by spaces.
    local t = {};
    for target in Plugin.IterateChildDependencies(libTarget) do
        if target.CppFile ~= nil then
            t[#t + 1] = getDepName(self, target)  -- "MACARONI_UNIT_TARGET_" .. target:GetCId()
        end
    end
    return table.concat(t, "\n        ");
end

function allTargets(self, libTarget)
    -- Returns a string with all dependent targets of a lib seperated by spaces.
    local t = {};
    for target in Plugin.IterateDependencies(libTarget) do
        local dep = getDepName(self, target)
        if dep ~= nil then
            t[#t + 1] = dep
        end
        -- if target.CppFile ~= nil then
        --     t[#t + 1] = getDepName(self, target)  -- "MACARONI_UNIT_TARGET_" .. target:GetCId()
        -- end
    end
    return table.concat(t, "\n        ");
end

function dependencyProperties(self, target)
    -- Creates a string with every dependency of this target listed as a
    -- property.
    local t = {};
    for dep in Plugin.IterateDependencies(target) do
        t[#t + 1] = "<dependency>" .. dep:GetCId()
    end
    if target.Parent ~= nil then
        t[#t + 1] = getLibraryProperties(self, target.Parent)
    end

    return table.concat(t, "\n        ");
end

function includePaths(target)
    -- Returns a string for use in the jam file which contains include paths
    -- for all necessary headers.
    local t = {};
    for header in Plugin.IterateFiles(target.Headers) do
        t[#t + 1] = "<include>\"" .. header.AbsolutePathForceSlash .. "\""
    end
    return table.concat(t, "\n        ");
end

function includeDepPaths(self, target)
    local t = {};
    for depTarget in Plugin.IterateDependencies(target) do
        t[#t + 1] = "# " .. tostring(depTarget)
        if depTarget.TypeName == 'lib' and
           self.jamSupport[depTarget.ProjectVersion:GetCId()] then
            local dep = getDepName(self, depTarget);
            if dep == nil then
                error("No name found for " .. tostring(depTarget) .. ".")
            end
            t[#t + 1] = "<library>" .. dep .. " "
        elseif depTarget.Headers ~= nil then
            t[#t + 1] = includePaths(depTarget)
        else
            t[#t + 1] = "# ..."
        end
    end
    return table.concat(t, "\n        ");
end

function _writePlatformExeTarget(self, writer, target)

end

function writeExeTarget(self, writer, target)
    writer:WriteLine("# " .. target.Name);
    local platforms = {}
    if MACARONI_VERSION ~= "0.1.0.27" then
        platforms = target.Platforms;
    end
    if #platforms > 0 then
        -- First, make an alias so this will be ignored by default.
        writer:WriteLine("alias " .. getDepName(self, target) .. " : : ;");
    end
    writer:WriteLine("exe " .. getDepName(self, target));
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependenciesWithJamSupport(self, target));
    writer:WriteLine("    :   ");
    local flags = self.flags[target.Name];
    if flags ~= nil then
        writer:WriteLine(flags);
    end
    for i=1, #platforms do
        -- platform rules are specified in the arguments to this plugin
        local rule = self.platformRules[platforms[i]];
        if rule == nil then
            error("There is not custom key in platformRules for platform "
                  .. platforms[i] .. "!");
        end
        writer:WriteLine("        " .. rule
                         .. " # " .. platforms[i] .. " rule ");
    end
    writer:WriteLine("    ;");
end

function writeExeTargets(self, writer)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "exe")
    do
        writeExeTarget(self, writer, target)
    end
end

function writeInstalRuleForExes(self, writer)
    writer:WriteLine([[install exe : ]])
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "exe")
    do
        writer:WriteLine(getDepName(self, target));
    end
     writer:WriteLine('\n' .. [[
    : <install-dependencies>on <install-type>EXE
             <install-type>LIB ; ]]);
end

function writeLibTarget(self, writer, lib)
    writer:WriteLine("# " .. lib.Name);
    writer:WriteLine("lib " .. getDepName(self, lib));
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependenciesWithJamSupport(self, lib));
    writer:WriteLine("    :   ");
    local flags = self.flags[lib.Name];
    if flags ~= nil then
        writer:WriteLine(flags);
    end
    writer:WriteLine("    ;");
end

function writeLibTargets(self, writer)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "lib")
    do
        writeLibTarget(self, writer, target)
    end
end

function writeUnitTargetVariant(self, writer, lib, properties)
    writer:WriteLine("unit-test " .. getDepName(self, lib) );
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependencies(self, lib));
    writer:WriteLine("    :  " .. properties);
    --TODO: What the hell is the right thing to put here? I've gone back
    --      and forth on this hundreds of times.
    writer:WriteLine("       <link>shared <define>BOOST_TEST_DYN_LINK ");
    writer:WriteLine("    ;");
end

function writeTestTarget(self, writer, lib)
    writer:WriteLine("# " .. lib.Name);
    if self.testVariants then
        -- Write multiple targets, all identical except for their
        -- properties.
        for i=1, #self.testVariants do
            writeUnitTargetVariant(self, writer, lib, self.testVariants[i])
        end
    else
        writeUnitTargetVariant(self, writer, lib, "")
    end
end

function writeTestTargets(self, writer)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "test")
    do
        writeTestTarget(self, writer, target)
    end
end

function writeUnitTargets(self, writer) -- , libTarget)
    -- Writes the info for every unit target in the project.
    --for target in Plugin.IterateChildDependencies(libTarget) do
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "unit")
    do
        local cppFile = target.CppFile
        if cppFile ~= nil then
            writer:WriteLine("# " .. target.Name);
            writer:WriteLine("obj " .. getDepName(self, target) ..  "\n"
                .. "    :   \"" .. target.CppFile.AbsolutePathForceSlash
                .. "\"\n"
                .. "    :   " .. dependencyProperties(self, target)  .. "\n"
                .. ";");
        end
    end
    local explained = false
    --for target in Plugin.IterateChildDependencies(libTarget) do
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "unit")
    do
        local cppFile = target.CppFile
        if cppFile == nil then
            if not explained then
                writer:WriteLine("\n# Additional child target(s) (not handled "
                                 .. "due to no cppFile):");
                explained = true
            end
            writer:WriteLine("# Skipping " .. target:GetShortCId())
        end
    end
end

function createLibraryProperties(libTarget)
    -- Figures out all the junk that should appear for a library target in
    -- the dependencies.
    local value = [[# When linking dynamically, define the following flag:
        <link>shared:<define>]] .. LibraryDynLink(libTarget) .. [[=1 ]]
        .. '\n      # Default LibraryProject Headers        ';
        -- The include paths can go here, but its easiest to just add them
        -- as project requirements.
        --
        --.. includePaths(libTarget) .. '\n' .. includeDepPaths(libTarget)
    return value
end


function getLibraryProperties(self, libTarget)
    -- Looks up the dependencies for a library target, or creates and caches it.
    local value = self.libTargetDeps[libTarget.Name]
    if value == nil then
        value = createLibraryProperties(libTarget)
        self.libTargetDeps[libTarget.Name] = value
    end
    return value
end

function writeBoostFile(self)
    debugLine(self, "Creating Jamroot.jam file at " ..
              self.jamroot.AbsolutePathForceSlash .. ".");
    local writer = self.jamroot:CreateFile();

    writer:WriteLine([[
# Generated by Macaroni.
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ~~ Library Info ~~
# Group   : ]] .. self.projectVersion.Project.Group.Name .. '\n' ..[[
# Name    : ]] .. self.projectVersion.Project.Name .. '\n' .. [[
# Version : ]] .. self.projectVersion.Version .. '\n' .. [[
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

import boost ;
import path ;
using testing ;

]]);

    writeUseStatements(self, writer);

    if self.defaultLib ~= nil then
        writer:WriteLine("# Default Project Library : "
                          .. self.defaultLib.Name .. '\n');
    else
        writer:WriteLine("#Note: Use the defaultLib argument to make this "
                         .. "project usable as a dependency.");
    end

    if self.defaultLib ~= nil then
        local extraFlags = self.requirements or '';
        writer:WriteLine([[

project ]] ..
'\n    ' .. self.projectVersion:GetCId() ..
'\n' .. [[
    :   requirements ]] .. '\n' ..   -- not sure if this should
                                     -- be "usage-requirements"
    includePaths(self.defaultLib) .. '\n'
    .. includeDepPaths(self, self.defaultLib) .. '\n' ..
    extraFlags .. '\n' ..
[[  :   usage-requirements ]] .. '\n' ..   -- not sure if this should
                                     -- be "usage-requirements"
    includePaths(self.defaultLib) .. '\n'
    .. includeDepPaths(self, self.defaultLib) .. '\n' ..
    createLibraryProperties(self.defaultLib) .. '\n' ..
[[
    ;

alias library_dependencies
    :    ;

]]);
    else
        writer:WriteLine([[
#Note: Use the defaultLib argument to make this project usable as a dependency.
        ]]);
    end  -- end if defaultLib ~= nil

    writer:WriteLine("\n# Units\n# -----\n")

    writeUnitTargets(self, writer); -- self.target);

    writer:WriteLine("\n# Libraries\n# ---------\n")

    writeLibTargets(self, writer);

    writer:WriteLine("\n# Tests\n# ---------\n")

    writeTestTargets(self, writer);

    writer:WriteLine("\n# Executables\n# ---------\n")

    writeExeTargets(self, writer);

    writer:WriteLine("\n# Makes the EXEs easier to run\n# ---------\n")
    writeInstalRuleForExes(self, writer);

    writer:Close();
end

function boostSystemProperties()
    if (properties.boost ~= nil and properties.boost.current ~= nil) then
        local bprops = properties.boost.current;
        return bprops;
    end
    return nil;
end

function createDependencyList(library)
    local rtn = {};
    local deps = library.Dependencies;
    for k, v in pairs(deps) do
        local dep = v;
        local jamDir = dependencyJamDir(v);
        if (jamDir ~= nil) then
            local element = { };
            element.jamDir = jamDir.AbsolutePathForceSlash;
            element.name = createProjectName(dep);
            rtn[#rtn + 1] = element;
        end
    end
    return rtn;
end

function initializeExtraArgs(args)
    args = args or {}
    args.excludePattern = args.ExcludePattern or ".svn";
    args.extraTargets = args.ExtraTargets or "";
    args.libraryRequirements = args.LibraryRequirements or "";
    args.tests = args.Tests or {};
    return args;
end


function findFilePath(sources, file)
    -- Given a series of directories, finds the file... if such a file exists!
    for i, v in ipairs(sources) do
        local fullPath = v:NewPathForceSlash(file)
        if fullPath.Exists then
            return fullPath.GetAbsolutePathForceSlash;
        end
    end
    return nil;
end

function createProjectDef(library)
    return "MACARONI_LIB_" .. createProjectName(library);
end

function createProjectName(library)
    return cleanUpName(library.Group) .. "____"
        .. cleanUpName(library.Name) .. "____"
        .. cleanUpName(library.Version);
end

function cleanUpName(str)
    return string.gsub(str, "_", "_u").gsub(str, " ", "_s");
end

function dependencyJamDir(d)
    local success, path = pcall(d.FindInstallPath, d);
    if (success and path ~= nil) then
        local pathText = path.AbsolutePathForceSlash;
        local jamroot = path:NewPathForceSlash('target/jamroot.jam');
        if (jamroot.Exists) then
            return path:NewPathForceSlash('target');
        end
    end
    return nil;
end


-- Copy all .C, .CPP, .H and .HPP files to dir.=
function copyCppSource(regEx, src, dst)
    local srcs = src:GetPaths(regEx);
    for i = 1, #srcs do
        local child = srcs[i];
        if (not child.IsDirectory) then
            debugLine(self, tostring(child.AbsolutePathForceSlash)
                            .. " ... " .. tostring(dst.AbsolutePathForceSlash));
            --src:CreateDirectory();
            child:CopyToDifferentRootPath(dst);
        else
            -- May not be necessary
            -- iterateDir(regEx, child, dst);
        end
    end
end

function isBoostDepProject(depProject)
    --TODO: Get some kind of annotation feature onto Libraries to eliminate
    --      this hack.
    local pName = depProject.Name
    return "Boost" == string.sub(pName, 1, 5)
end

function writeUseStatementForGenericBBProject(self, writer, depProject)
    --TODO: Find some way to stick the location of the jamroot onto
    -- the target itself so this hardcoded path nonsense can be
    -- avoided.
    local jamDir = findOrCreateInstallPath(depProject)
        :NewPathForceSlash("/target")
    self.jamSupport[depProject:GetCId()] = jamDir.Exists
    if jamDir.Exists then
        local jamroot = jamDir:NewPathForceSlash("/jamroot.jam")
        if jamroot.Exists then
            writer:WriteLine("    use-project /" .. getProjectName(depProject)
                .. '\n' .. "        : "
                .. jamDir.GetAbsolutePathForceSlash .. "\n        ; ");
            return true;
        else
            writer:WriteLine("# No jamroot.jam : " .. tostring(depProject))
        end
    end
    return false;
end

function writeUseStatementForBoost(self, writer, depProject)
    if not isBoostDepProject(depProject) then
        return false;
    end
    self.jamSupport[depProject:GetCId()] = true;
    if self.BoostUseVersion ~= nil then
        if self.BoostUseVersion == depProject.Version then
            writer:WriteLine("    # see boost.use-project above ")
            return true;
        else
            error("Can't mix Boost versions! Version " .. self.BoostUseVersion
                  .. " was already in use, but an attempt was made to use "
                  .. " a different version for " .. tostring(depProject) .. ".");
        end
    end
    writer:WriteLine("    boost.use-project " .. depProject.Version .. " ;");
    self.BoostUseVersion = depProject.Version;
    return true;
end

function writeUseStatementForUnknownProject(self, writer, depProject)
    writer:WriteLine("    #~<( I don't know how to 'use' this dependency. )");
end

function writeUseStatement(self, writer, depProject)
    -- Determine the correct way to use a dependency project and write it.
    writer:WriteLine("# " .. tostring(depProject))
    return writeUseStatementForBoost(self, writer, depProject) or
        writeUseStatementForGenericBBProject(self, writer, depProject) or
        writeUseStatementForUnknownProject(self, writer, depProject);
end

function writeUseStatementsForType(self, writer, type)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      type) do
        local t = {};
        -- print(Plugin.IterateDependencyProjects)
        for depProject in Plugin.IterateDependencyProjects(target) do
            writeUseStatement(self, writer, depProject)
        end
    end
end

function writeUseStatements(self, writer)
    self.jamSupport = {}
    writeUseStatementsForType(self, writer, "lib")
    writeUseStatementsForType(self, writer, "exe")
    writeUseStatementsForType(self, writer, "test")
end

function Prepare(library, sourcePaths, outputPath, installPath, extraArgs)
end

function printDependencyProjectIncludes(writer, library)
    writer:Write([[
# Dependencies, in the form of Boost Build projects. ]]);
    local deps = library.Dependencies;
    writer:Write("# There are " .. #deps .. " dependencies.\n");
    for k, v in pairs(deps) do
        local dep = v;
        local jamDir = dependencyJamDir(v);
        writer:Write("# " .. dep.Group .. " // " .. dep.Name .. "//" .. dep.Version .. "\n");
        if (jamDir == nil) then
            writer:Write("# nil\n");
        else
            writer:Write("use-project /" .. createProjectName(dep)
                .. " : \"" .. jamDir.AbsolutePathForceSlash .. "\" ;\n");
        end
    end
end
