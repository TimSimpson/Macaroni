--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Cpp/LibraryConfigGenerator";
require "Cpp/NodeInfo";
require "Cpp/UnitFileGenerator";
require "Log";
require "Plugin"
local ProjectGenerator = require "Macaroni.Generators.Cpp.Project.ProjectGenerator"

local Path = require "Macaroni.IO.Path";

if MACARONI_VERSION ~= "0.1.0.23" then
    require "Macaroni.Model.Project.Target";
end


CPP_GENERATE_VERBOSE = false


function trace(msg)
    --print(oldDebugTraceback(0, ni2))
    if msg ~= nil then
        print("TRACE:" .. msg)
    end
    local level = 2
      while true do
        local info = debug.getinfo(level, "Sl")
        if not info then break end
        if info.what == "C" then   -- is a C function?
          print(level, "C function")
        else   -- a Lua function
          print(string.format("[%s]:%d",
                              info.short_src, info.currentline))
        end
        level = level + 1
      end
end
-- Monkey patch default trace back.
--oldDebugTraceback = debug.traceback
--debug.traceback = onError


-- GLOBAL Log definition.
log = log.Init("Cpp");
log.Write = function(self, msg)
    --print("[CPP]:" .. msg);
end;
log.Error = function(self, msg)
    print("[CPP::!!!!!!!!!!]:" .. msg);
end;

function Validate(self)
    Plugin.Check(self.projectVersion ~= nil, "Missing argument 'projectVersion'.")
    Plugin.Check(self.path ~= nil, "Missing argument 'path'.")
    Plugin.Check(self.path.AbsolutePath ~= nil, [["path" argument looks ]] ..
                                        [[wrong. Is this a path or a string?]])
end

function Generate3(self)
    local debug = true;
    ProjectGenerator.Generate(self.projectVersion,
                              self.path:CreateWithCurrentAsRoot(),
                              debug)
end

function Generate2(self)
    -- The newer version used by the project system.

    -- Replace log.Write with the output.DebugLine method passed in.
    -- Because each generator uses it's own Lua environment we can use globals
    -- like this, even though it's still kind of dirty.
    local oldOutput = self.output
    log.Write = function(self, msg)
        if oldOutput ~= nil then
            oldOutput:DebugLine(msg)
        end
    end

    log.Error = function(self, msg)
        if oldOutput ~= nil then
            oldOutput:ErrorLine(msg)
        end
    end


    Validate(self)
    if self.defaultLib == nil then
        for i=1, #self.projectVersion.Targets do
            local target = self.projectVersion.Targets[i]
            if target.Name == "lib" then
                self.defaultLib = target
            end
        end
    end
    -- Iterate all higher level Targets, generating each one.
    for lib in Plugin.IterateProjectVersionTargets(self.projectVersion, "lib")
    do
        Generate(lib, self.path)
    end
    for exe in Plugin.IterateProjectVersionTargets(self.projectVersion, "test")
    do
        Generate(exe, self.path)
    end
    for exe in Plugin.IterateProjectVersionTargets(self.projectVersion, "exe")
    do
        Generate(exe, self.path)
    end
end


function Generate(library, path)
    -- Install dep watcher.
    local pchDeps = {}
    DependencyUseObserver.notify = function(_target, nodeInfo, heavy, text)
        local node = nodeInfo.node
        if heavy and
            (node.Element == nil or not node.Element:OwnedBy(library)) then
            pchDeps[node.FullName] = text
        end
    end

    -- Legacy mode: Iterate nodes and write files.
    if MACARONI_VERSION == "0.1.0.23" then
        log:Write("Generating H Files_.\n");
        local hGen = HFileGenerator.new(library);
        log:Write("Adonde esta? " .. tostring(library) .. " !");
        hGen:iterateNodes(library.Context.Root.Children, path);
        log:Write("Generating Cpp Files\n");
        local cppGen = CppFileGenerator.new(library);
        cppGen:iterateNodes(library.Context.Root.Children, path);
        log:Write("End of Cpp.lua\n");
    else
    -- New mode- Assume units have been created. Write files per unit.
        local rootGenPath = Path.New(path.AbsolutePath)
        local libGen = LibraryTargetGenerator.new(library);
        libGen:IterateUnits(library, rootGenPath);
    end

    -- Write a centralized header file which can set macros for build options.
    if BoostConfigIsAvailable(library) then -- .Context) then
        lcg = LibraryConfigGenerator.new(library);
        lcg:writeFile(path);
        lcg:writePchFile(path, pchDeps);
    end
end


function GetMethod(name)
    if name == "Generate" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Generate C++ code for target "
                                      .. tostring(args.target)
                                      .. "to the directory "
                                      .. tostring(args.path) .. ".");
            end,
            Run = function(args)
                -- print("")
                -- print("TARGET=" .. tostring(args.target));
                -- print("PATH=" .. tostring(args.path));
                -- print("PATH.NewPath=" .. tostring(args.path.NewPath));

                Generate3(args)
            end
        }
    end
    if name == "Install" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Save all C++ source code for "
                                      .. tostring(args.projectVersion)
                                      .. "to {library install path}/Source.")
            end,
            Run = function(args)
                print("USING TARGET=" .. tostring(args.projectVersion))
                local projectVersion = args.projectVersion
                local path = findOrCreateInstallPath(args.projectVersion)
                local dst = path:NewPathForceSlash("Source")
                    :CreateWithCurrentAsRoot();
                local Path = Macaroni.IO.Path;
                print("Destination:" .. tostring(dst))
                print("Copying Sources:")
                for i=1, #args.paths do
                    local src = args.paths[i]:CreateWithCurrentAsRoot();
                    print("\t" .. tostring(i) .. "=" ..tostring(src))
                    Path.CopyDirectoryContents(src, dst)
                end
                -- print("")
                -- print("TARGET=" .. tostring(args.target));
                -- print("PATH=" .. tostring(args.path));
                -- print("PATH.NewPath=" .. tostring(args.path.NewPath));

                --Generate2(args)
            end
        }
    end
    return nil;
end
