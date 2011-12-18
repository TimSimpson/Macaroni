
EnabledLoggers =
{
	"Cpp",
	"LuaGlue",
}

log =
{
	Write = function(msg) end,

	Init = function(name)
		if (EnabledLoggers[name] ~= nil) then
			log.Write = function(msg)
				print("[" .. name .."]:" .. msg);
			end;
		else
			log.Write = function(msg)
			end;
		end
		return log
	end,
}
