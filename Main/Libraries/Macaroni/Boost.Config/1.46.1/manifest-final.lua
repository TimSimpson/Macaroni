id =
{
	group	= [[Macaroni]],
	name	= [[Boost-smart_ptr]],
	version	= [[1.46.1]]
}

description = [[
Boost Config library. This is a series of header files aiming to make writing
portable code with more configurations easier.
]]

sources = { [[Source]] }

output = [[Cpp]]

function prepare()
	run("PrepareBoostBuild");	
end