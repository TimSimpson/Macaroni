
sources = { "src" }
output = "target"

function generate()   
	run("HtmlView") 
    run "Cpp"    
end

function build()
	run("BoostBuild", { 			
		ExcludePattern = "Tests.cpp",		
		ExtraTargets = [[						
		  	exe Tests 
		  		:	library
		  		    #test_dependencies
		  			../src/Tests.cpp
		  		:  <target-os>windows:<linkflags>/LIBPATH:"]]
            .. properties.boost.current["path"] .. [[/stage/lib"
		  		;		  	
		  ]],
		Link="static",
	  	Shared = True,
	  	--Tests = {"Tests.cpp"}
	})
end