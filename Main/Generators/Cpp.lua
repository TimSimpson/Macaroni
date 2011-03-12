--package.path = "F:/Lp3/Projects/Macaroni/Code/trunk/Macaroni/Macaroni/Debug/Generators/?.lua"
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";
require "Log";


function Generate(library, path)
	log = log.Init("Cpp");
	log.Write = function(self, msg) 			
			print("[CPP]:" .. msg);
		end;
    log:Write("Generating H Files_.\n");
    local hGen = HFileGenerator.new(library);
    log:Write("Adonde esta? " .. tostring(library) .. " !");
    hGen:iterateNodes(library.Context.Root.Children, path); 
    log:Write("Generating Cpp Files\n");
    local cppGen = CppFileGenerator.new(library);
    cppGen:iterateNodes(library.Context.Root.Children, path); 
    log:Write("End of Cpp.lua\n");
end
