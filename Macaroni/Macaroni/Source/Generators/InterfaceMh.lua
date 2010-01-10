package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni/MhFileGenerator";

-- Generates the Macaroni header file.  This single files contains abbreviated
-- info about an entire library.

function Generate(context, path)
    print "Generating Macaroni Header File\n";
    local gen = MhFileGenerator.new(path);
    gen:iterateNodes(context.Root.Children);     
end
