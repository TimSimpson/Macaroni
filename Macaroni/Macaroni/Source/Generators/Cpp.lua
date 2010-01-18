--package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";

function Generate(library, path)
    print "Generating H Files\n";
    local hGen = HFileGenerator.new();
    hGen:iterateNodes(library.Context.Root.Children, path); 
    print "Generating Cpp Files\n";
    local cppGen = CppFileGenerator.new();
    cppGen:iterateNodes(library.Context.Root.Children, path); 
end
