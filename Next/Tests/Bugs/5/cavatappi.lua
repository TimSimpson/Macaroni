
sources = { "src" }
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