id =
{
	group	= [[Macaroni]],
	name	= [[Boost-smart_ptr]],
	version	= [[1.42.0]]
}

description = [[Boost smart_ptr library.]]

sources = { [[Source]] }

output = [[Cpp]]

function prepare()
	run("PrepareBoostBuild");	
end