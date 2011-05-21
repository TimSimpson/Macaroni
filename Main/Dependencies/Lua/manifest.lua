id =
{
	group	= [[Macaroni]],
	name	= [[Lua]],
	version	= [[5.1.4]]
}

description = [[Lua library compiled as C++]]

sources = { [[Source]] }

output = [[Target]]

jamArgs = 
{ 	
	ExcludePattern = "luac.cpp lua.cpp",
	ExtraTargets = 
	[[	    
	  	exe lua 
	  		:	library 
	  			lua.cpp
	  		;
	]],
	Shared = True
};
	
function build()
	run("CopySource");
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs);
end