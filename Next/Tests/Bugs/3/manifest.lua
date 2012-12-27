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
	
	UPDATE: While tracing code I realized what the problem is... and its a 
	big deal!
	
	With:
	class Access;
		
	boost::shared_ptr<Access> AccessPtr;
	is different than
	std::vector<Access> AccessList;
	
	In the later case, you *need* the heavy dependency to Access to define
	AccessList.
	But for AccessPtr, you don't, probably because Boost turns it into the
	type argument into a pointer and makes it possible to use a forward 
	reference since they're smart like that.
	
	The issue is I have no idea how to tell Macaroni that in some cases
	Node used by a Type can be a light dependency.
	
	If I could somehow tell it that "boost::smart_ptr" can have nothing
	but light deps that's be cool.'
	
]]

cavatappi("cavatappi.lua");