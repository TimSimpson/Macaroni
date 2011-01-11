package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Debug/Generators/?.lua"
require "Cpp/HFileGenerator";
require "Log";

function Generate(context, path)
	log.Init("Cpp");
    log:Write("Generating H Files\n");
    
    local hGen = HFileGenerator.new();
    hGen:iterateNodes(context.Root.Children, path); 
end
