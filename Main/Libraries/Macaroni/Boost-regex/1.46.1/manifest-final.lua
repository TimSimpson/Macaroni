id =
{
	group	= [[Macaroni]],
	name	= [[Boost-regex]],
	version	= [[1.46.1]]
}

description = [[Boost regex library.]]

sources = { [[Source]] }

output = [[Cpp]]

function prepare()
	run("PrepareBoostBuild");	
end