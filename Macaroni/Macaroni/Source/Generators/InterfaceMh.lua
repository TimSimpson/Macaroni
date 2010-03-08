require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";
require "Macaroni/MhFileGenerator";

-- Generates the Macaroni header file.  This single files contains abbreviated
-- info about an entire library.

targetLibrary = nil;

function Generate(library, path)
    print "Generating Macaroni Header File\n";
    targetLibrary = library;
    local gen = MhFileGenerator.new(path);
    gen:iterateNodes(library.Context.Root.Children);     
end
