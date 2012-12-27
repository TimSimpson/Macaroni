id =
{
    group="Macaroni.Examples",
    name="Hello",
    version="1.0.0.0"
}

description= [[  
         _________________________________________
    '_' < A quick example of how to use Macaroni. )
         -----------------------------------------
]]

sources = { "source" }
output = "target"

dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group="Macaroni", name="Boost.Config", version="1.46.1"}

function generate()
	run("cpp")
	run("htmlview")
end

function build()
	run("BoostBuild", { 	
		ExcludePattern = ".svn *Main.cpp",	
		ExtraTargets = [[				
		  	exe Hello 
		  		:	library
		  			../target/Main.cpp	  					
		  		;
		  ]]
	})
end

function install()
	run("Archive", { directory=manifestDirectory, 
	                 regex=[[\.(cpp|h|mcpp|lua)?$]]} );
end