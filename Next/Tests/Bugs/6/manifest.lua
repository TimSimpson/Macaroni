upper = getUpperLibrary();

id =
{
    group=upper.Group,
    name= upper.Name .. ".4",
    version="0.1.0.20",
    author="Tim Simpson"
}

dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}

description= [[  
	Its not possible to use two nodes with the same name even when their
	full name is different. In C++ you can of course just reference them
	in different ways, but Macaroni is forcing the "using" statement to
	appear which causes problems.
	
	To fix this, Macaroni needs some kind of alias functionality on its
	import statement. So for example, you'd write:
	
	~import Kennel::Dog;
	~import Village::Dog as VDog;
	
	Then Macaroni would define the header file as usual, but in the C++ file
	create a macro for VDog:
	
	#define Village::Dog VDog
	
	That way users could use "VDog" in their code.
	
	Another issue though is that this forces the C++ generation of
	classes to either specify the full name of every node, which would
	make it even uglier, or figure out that "VDog" is an acceptable
	alias and use it.
	
	There is also additional work on the parser side, where "VDog" would
	need to be interpretted as "Village::Dog" every place it is 
	seen.
	
	And of course, there's more trouble if this is used in inline methods
	of the hidden header. Then this has to occur up there which could be
	problematic.
]]

sources = { "src/test", "src/main" }
--source "src/test"
--source "src/main"

output = "target"

function generate()   
	run("HtmlView") 
    run "Cpp"    
end

function build()
	run("BoostBuild", { 	
		ExcludePattern = "Tests.cpp",		
	  	Shared = True,
	  	Tests = {"Tests.cpp"}
	})
end

cavatappi("cavatappi.lua");