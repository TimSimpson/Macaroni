upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name=upper.Name .. ".1",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
	When a build phase method is not found, Macaroni should not fail the build.
	Instead it should act as if the build phase method is empty.
]]

cavatappi("cavatappi.lua");