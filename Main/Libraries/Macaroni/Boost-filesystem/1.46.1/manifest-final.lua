id =
{
	group	= [[Macaroni]],
	name	= [[Boost-filesystem]],
	version	= [[1.46.0]]
}

description = [[Boost filesystem library.]]

sources = { [[Source]] }

output = [[Cpp]]

function prepare()
	run("PrepareBoostBuild");	
end