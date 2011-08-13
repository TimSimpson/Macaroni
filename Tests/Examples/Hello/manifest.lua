id =
{
    group="Macaroni.Examples",
    name="Hello",
    version="1.0.0.0"
}

description= [[  
         ________________________
    '_' < Sorry, I know nothing. )
         ------------------------
]]

sources = { "source" }
output = "target"

dependency {group="Macaroni", name="CppStd", version="2003"}

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