-- Extends Site plugin but focuses on Rst files.

require "Site"

local oldGetMethod = GetMethod

function GetMethod(name)
    if name == "Build" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Generate Restructured Text files.")
            end,
            Run = function(args)
                local runner = oldGetMethod("build")
                args.extension = args.extension or "rst"
                args.outputSubPath = args.outputSubPath or "/"
                -- outputSubPath is a bit useless as most calls to this plugin
                -- will want the absolute paths to according to the project's
                -- base.
                return runner.Run(args)
            end
        }
    elseif name == "Site" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Call sphinx to build webpage.")
            end,
            Run = function(args)
                local target = args.outputPath
                local confFile = target:NewPathForceSlash("docs/conf.py")
                if not confFile.Exists then
                    local writer = confFile:CreateFile()
                    writer:WriteLine([[
import sys
import os

extensions = []
master_doc = 'index']]);
                end
                local src = target:NewPathForceSlash("docs").AbsolutePath
                local dst = target:NewPathForceSlash("site").AbsolutePath
                args.output:DebugLine("Running Sphinx...")
                local success, exit, number =
                    os.execute("sphinx-build  -b html " .. src .. " " .. dst)
                if (not success or exit ~= "exit" or number ~= 0) then
                    args.output:ErrorLine("Failure running Sphinx! ")
                    error("Failure running Sphinx!")
                end
            end,
        }
    end
    return nil;
end
