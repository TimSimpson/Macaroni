package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Debug/Generators/?.lua"
require "Cpp/HFileGenerator";

function Generate(context, path)
    print "Generating H Files\n";
    
    local hGen = HFileGenerator.new();
    hGen:iterateNodes(context.Root.Children, path); 
end
