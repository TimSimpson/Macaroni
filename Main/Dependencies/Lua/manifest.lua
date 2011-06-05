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
	LibraryRequirements =
	[[
		<link>shared:<define>LUA_BUILD_AS_DLL
	]],
	Shared = True
};
	
function generate()
	run("CopySource")
    run("InterfaceMh")
end

function build()
	run("CopySource");
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs);
	run("InterfaceMh")
end