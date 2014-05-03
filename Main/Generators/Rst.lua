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
    end
    return nil;
end
