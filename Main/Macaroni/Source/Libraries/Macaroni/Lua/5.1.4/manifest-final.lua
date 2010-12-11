id =
{
	group	= [[Macaroni]],
	name	= [[Lua]],
	version	= [[5.1.4]]
}

description = [[Lua library compiled as C++]]

sources = { [[Source]] }

output = [[Cpp]]

function prepare()	
	run("PrepareBoostBuild", {properties.lua.["5.1.4"]});	
end