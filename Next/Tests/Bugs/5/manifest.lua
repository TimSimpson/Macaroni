upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".3",
    version="0.1.0.20",
    author="Tim Simpson"
}

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}

description= [[ 
	The parser doesn't allow comments in certain places within Function 
	argument lists.
]]

cavatappi("cavatappi.lua");