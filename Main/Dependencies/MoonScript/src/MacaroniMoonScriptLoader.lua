-- Disclaimer:
-- This code is mostly copied from the moon script (no pun intended) written
-- by Leaf Corcoran. However it was modified a bit to make it feel like a first
-- class citizen in Macaroni so anything disgusting you find here is probably
-- something I added.
--
-- It's possible that moonscript.dofile is all that needs to be called but
-- I can't seem to get the MoonScript source in the stack trace that way
-- (maybe this is due to something nonstandard in Macaroni).
-- Writing this one extra lua file was easier than doing the equivalent steps
-- with the Lua C API. :)

local moonscript = require "moonscript"
local util = require "moonscript.util"
local errors = require "moonscript.errors"

function RunMoonScriptFile(filePath, errorOutputMethod)
    -- Runs a Moon Script file. Needs a filePath and a method to output errors
    -- if something goes wrong.
    errorOutputMethod = errorOutputMethod or print
    local moonscript_chunk, lua_parse_error
    local passed, err = pcall(function()
      moonscript_chunk, lua_parse_error = moonscript.loadfile(
            filePath,
            { implicitly_return_root = false })
    end)
    if not passed then
      errorOutputMethod(err)
    else
      if not moonscript_chunk then
        if lua_parse_error then
          errorOutputMethod(lua_parse_error)
          error(lua_parse_error)
        else
          local msg = "Can't find file: " .. filePath
          errorOutputMethod(msg)
          error(msg)
        end
      else
        xpcall(function() moonscript_chunk() end, function(_err)
            err = _err
            trace = debug.traceback("", 2)
        end)
        if err then
            local truncated = errors.truncate_traceback(util.trim(trace))
            local rewritten = errors.rewrite_traceback(truncated, err)
            if rewritten then
                errorOutputMethod(rewritten)
                error(err)
            else
                errorOutputMethod(table.concat({
                  err,
                  util.trim(trace)
                }, "\n"))
                error(err)
            end
        end
      end
    end
end
