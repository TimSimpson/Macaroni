upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".3",
    version="0.1.0.14",
    author="Tim Simpson"
}

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}

description= [[  
	There is something wrong with how Macaroni finds dependencies and writes 
	them into header files, leading to code files with circular includes that
	don't compile.
	In this example, there are several classes with dependencies on each other.
	While there is a way to write this code correctly, Macaroni currently 
	does not.
]]

cavatappi("cavatappi.lua");