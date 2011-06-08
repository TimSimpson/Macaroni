
sources = { "src" }
output = "target"

function generate()    
    run "Cpp"    
end

function build()
	run("BoostBuild")
end