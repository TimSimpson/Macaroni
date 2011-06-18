
sources = { "src" }
output = "target"

function generate()   
	run("HtmlView") 
    run "Cpp"    
end

function build()
	run("BoostBuild")
end