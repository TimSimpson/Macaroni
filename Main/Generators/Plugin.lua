-- Helper functionality for Plugins.


Plugin =
{
    Check = function(con, msg)
        -- If con is false calls error with msg, blaming the caller.
        if (not con) then
            error(msg, 3);  -- Blame caller of the caller to "check."
        end
    end,


    CreateFakeLog = function()
        -- Creates a log object which does nothing.
        local nothing = function(msg)
        end
        return {
            Error = nothing,
            ErrorLine = nothing,
            Write = nothing,
            WriteLine = nothing,
        }
    end,


    IterateFiles = function(files)
        -- Iterate headers for a target using for.
        local i = 0
        return function ()
            i = i + 1
            if i <= #files then
                return files[i]
            end
        end
    end,

    IterateDependencies = function(target)
        local i = 0
        local deps = target.Dependencies;
        return function ()
            i = i + 1
            if i <= #deps then
                return deps[i]
            end
        end
    end,

    IterateChildDependencies = function(target)
        itr = Plugin.IterateDependencies(target)
        return function()
            while true do
                rtn = itr()
                if rtn == nil or rtn.Parent == target then
                    return rtn
                end
            end
        end
    end
}
