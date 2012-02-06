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
require "Macaroni.Model.Library";
require "Log";
require "Macaroni.IO.Path";
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


function Generate(library, path)
    log = log.Init("Cpp");
    log.Write = function(self, msg)
          --print("[CPP]:" .. msg);
    end;
    if BoostConfigIsAvailable(library.Context) then
        lcg = LibraryConfigGenerator.new(library);
        lcg:writeFile(path);
    end
    log:Write("Generating H Files_.\n");
    local hGen = HFileGenerator.new(library);
    log:Write("Adonde esta? " .. tostring(library) .. " !");
    hGen:iterateNodes(library.Context.Root.Children, path);
    log:Write("Generating Cpp Files\n");
    local cppGen = CppFileGenerator.new(library);
    cppGen:iterateNodes(library.Context.Root.Children, path);
    log:Write("End of Cpp.lua\n");
end


function printTable(tbl)
    print("TABLE")
    for key, value in pairs(tbl) do
        print(tostring(key) .. " = " .. tostring(value))
        print("\t" .. tostring(type(value)))
    end
    print("END TABLE")
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
            Install = function(args)
                -- By default, Macaroni itself will copy all artifacts
                -- and source to the destination directory "Source".
            end,
            Run = function(args)
                -- print("")
                -- print("TARGET=" .. tostring(args.target));
                -- print("PATH=" .. tostring(args.path));
                -- print("PATH.NewPath=" .. tostring(args.path.NewPath));

                Generate(args.target, args.path)
            end
        }
    end
    return nil;
end
