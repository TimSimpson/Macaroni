-- Helper functionality for Plugins.


Plugin =
{
    Check = function(con, msg)
        -- If con is false calls error with msg, blaming the caller.
        if (not con) then
            error(msg, 3);  -- Blame caller of the caller to "check."
        end
    end,


    CreateFakeLog = function() do
        -- Creates a log object which does nothing.
        local nothing = function(msg) do
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
            if i > #files then
                return files[i]
            end
        end
    end
}
