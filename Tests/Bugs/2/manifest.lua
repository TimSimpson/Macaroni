upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".2",
    version="0.1.0.6",
    author="Tim Simpson"
}

description= [[  
	When a class type starts with "const" the Parser thinks its the keyword const
	and breaks.
]]

cavatappi("cavatappi.lua");