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
	When there's more than one source directory the jamroot doesn't 
	include it.	
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