package.path = "C:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";

function Generate(context, path)
    print "Generating H Files\n";
    local hGen = HFileGenerator.new();
    hGen:iterateNodes(context.Root.Children, path); 
    print "Generating Cpp Files\n";
    local cppGen = CppFileGenerator.new();
    cppGen:iterateNodes(context.Root.Children, path); 
end
