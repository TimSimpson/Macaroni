--package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";

log = function(msg)
	--print("[Cpp.lua]:" .. msg);
end

function Generate(library, path)
	log = function(msg)
		print("[Cpp.lua]:" .. msg);
	end
    log("Generating H Files_.\n");
    local hGen = HFileGenerator.new(library);
    log("Adonde esta? " .. tostring(library) .. " !");
    hGen:iterateNodes(library.Context.Root.Children, path); 
    log("Generating Cpp Files\n");
    local cppGen = CppFileGenerator.new(library);
    cppGen:iterateNodes(library.Context.Root.Children, path); 
    log("End of Cpp.lua\n");
end
