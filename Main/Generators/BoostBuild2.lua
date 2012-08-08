require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";
require "Log"
require "Cpp/LibraryConfigGenerator";
require "Plugin"

Target = Macaroni.Model.Project.Target;

--require "Macaroni.IO.PathList";


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
                print("INSTALLING TO PATH " .. tostring(dst))
                for target in Plugin.IterateProjectVersionTargets(
                    self.projectVersion, "unit")
                do
                    local cppFile = target.CppFile
                    local hFile = target.HFile
                    print(tostring(target))
                    print("\t" .. tostring(cppFile))
                    print("\t" .. tostring(hFile))
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
end

function Generate(self)
    validateArgs(self)
    writeBoostFile(self)
end

function validateArgs(self)
    Plugin.Check(self.projectVersion ~= nil, "Missing argument 'projectVersion'.")
    Plugin.Check(self.jamroot ~= nil, "Missing argument 'jamroot'.")
    Plugin.Check(self.output ~= nil, "Missing argument 'output'.")
    self.Log = self.Log or Plugin.CreateFakeLog()
end

function getProjectName(projectVersion)
    return "MACARONI_PROJECT_" .. projectVersion:GetCId();
end

function getDepName(self, target)
    local prefix = ""
    if target.ProjectVersion:GetCId() ~= self.projectVersion:GetCId() then
        prefix = "/" .. getProjectName(target.ProjectVersion) .. "//"
    end
    if target.TypeName == "unit" then
        if target.CppFile ~= nil then
            return prefix .. "MACARONI_UNIT_TARGET_" .. target:GetCId();
        else
            return nil
        end
    end
    if target.TypeName == "lib" then
        return prefix .. "MACARONI_LIB_TARGET_" .. target:GetCId();
    end
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
            t[#t + 1] = "<library>" .. dep .. " "
        elseif depTarget.Headers ~= nil then
            t[#t + 1] = includePaths(depTarget)
        else
            t[#t + 1] = "# ..."
        end
    end
    return table.concat(t, "\n        ");
end

function writeExeTarget(self, writer, target)
    writer:WriteLine("# " .. target.Name);
    writer:WriteLine("exe " .. target:GetCId());
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependenciesWithJamSupport(self, target));
    writer:WriteLine("    :   # TODO: May need to set some compiler flags here.");
    writer:WriteLine("    ;");
    writer:WriteLine([[
install exe : ]] .. target:GetCId() .. '\n' .. [[
    : <install-dependencies>on <install-type>EXE
             <install-type>LIB ; ]]);
end

function writeExeTargets(self, writer)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "exe")
    do
        writeExeTarget(self, writer, target)
    end
end

function writeLibTarget(self, writer, lib)
    writer:WriteLine("# " .. lib.Name);
    writer:WriteLine("lib MACARONI_LIB_TARGET_" .. lib:GetCId());
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependenciesWithJamSupport(self, lib));
    writer:WriteLine("    :   # TODO: May need to set some compiler flags here.");
    writer:WriteLine("    ;");
end

function writeLibTargets(self, writer)
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "lib")
    do
        writeLibTarget(self, writer, target)
    end
end

function writeTestTarget(self, writer, lib)
    writer:WriteLine("# " .. lib.Name);
    writer:WriteLine("unit-test " .. lib:GetCId());
    writer:WriteLine("    :   # Sources:");
    writer:WriteLine(allDependencies(self, lib));
    writer:WriteLine("    :   # TODO: May need to set some compiler flags here.");
    writer:WriteLine("    ;");
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
            writer:WriteLine("obj MACARONI_UNIT_TARGET_" .. target:GetCId() .. "\n"
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
            writer:WriteLine("# Skipping " .. target:GetCId())
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
    log:Write("Creating Boost.Build file at " ..
              self.jamroot.AbsolutePathForceSlash .. ".");
    self.output:WriteLine("Creating Jamroot file at " .. tostring(self.jamroot)
                          .. ".");
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
        writer:WriteLine([[

project ]] ..
'\n    ' .. self.projectVersion:GetCId() ..
'\n' .. [[
    :   requirements ]] .. '\n' ..   -- not sure if this should
                                     -- be "usage-requirements"
    includePaths(self.defaultLib) .. '\n'
    .. includeDepPaths(self, self.defaultLib) .. '\n' ..
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


function Test(library, sources, outputPath, installPath, extraArgs)
    log.Init("BoostBuild");
    local cmdLine = "bjam"
    if (extraArgs.CmdLine ~= nil) then
        cmdLine = cmdLine .. " " .. extraArgs.CmdLine
    end
    cmdLine = cmdLine .. " " ..  outputPath.AbsolutePathForceSlash
    print(cmdLine)
    local rtnCode = os.execute(cmdLine)
    log:Write("BJAM return code = " .. rtnCode .. ".")
    if (rtnCode ~= 0) then
        error("Call to Boost.Build failed.")
        return false;
    end
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
            log:Write(tostring(child.AbsolutePathForceSlash) .. " ... " .. tostring(dst.AbsolutePathForceSlash));
            --src:CreateDirectory();
            child:CopyToDifferentRootPath(dst);
        else
            -- May not be necessary
            -- iterateDir(regEx, child, dst);
        end
    end
end

function writeUseStatements(self, writer)
    self.jamSupport = {}
    for target in Plugin.IterateProjectVersionTargets(self.projectVersion,
                                                      "lib") do
        local t = {};
        print(Plugin.IterateDependencyProjects)
        for depProject in Plugin.IterateDependencyProjects(target) do
            --TODO: Find some way to stick the location of the jamroot onto
            -- the target itself so this hardcoded path nonsense can be
            -- avoided.
            local jamDir = findOrCreateInstallPath(depProject)
                :NewPathForceSlash("/target")
            self.jamSupport[depProject:GetCId()] = jamDir.Exists
            if jamDir.Exists then
                local jamroot = jamDir:NewPathForceSlash("/jamroot.jam")
                if jamroot.Exists then
                    writer:WriteLine([[
use-project /]] .. getProjectName(depProject) .. '\n' .. [[
        : "]] .. jamDir.GetAbsolutePathForceSlash .. '"\n' .. [[
        ;]]);
                else
                    writer:WriteLine("# No jamroot.jam : " .. tostring(depProject))
                end
            end
        end
    end

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
