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
        Plugin.Check(files ~= nil, "Argument 'files' cannot be nil.")
        local i = 0
        return function ()
            i = i + 1
            if i <= #files then
                return files[i]
            end
        end
    end,

    IterateDependencies = function(target)
        -- Iterator for a Target's dependencies. Usable by for.
        return Plugin.IterateTargetList(target.Dependencies)
    end,

    IterateChildDependencies = function(target)
        -- Iterator for the child / dependencies of a Target. Usable by for.
        itr = Plugin.IterateDependencies(target)
        return function()
            while true do
                rtn = itr()
                if rtn == nil or rtn.Parent == target then
                    return rtn
                end
            end
        end
    end,

    IterateProjectVersionTargets = function(projectVersion, type)
        -- Iterator for Targets owned by a ProjectVersion.
        return Plugin.IterateTargetList(projectVersion.Targets, type)
    end,

    IterateTargetList = function(targetList, type)
        -- An iterator for a TargetList, usable by for.
        local i = 0
        if type == nil then
            return function ()
                i = i + 1
                if i <= #targetList then
                    return targetList[i]
                end
            end
        else
            return function()
                i = i + 1
                while i <= #targetList and targetList[i].TypeName ~= type do
                    i = i + 1
                end
                if i <= #targetList then
                    return targetList[i]
                end
            end
        end
    end,
}
