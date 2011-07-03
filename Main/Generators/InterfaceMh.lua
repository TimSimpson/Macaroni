require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";
require "Macaroni/MhFileGenerator";
require "Log";
-- Generates the Macaroni header file.  This single files contains abbreviated
-- info about an entire library.

targetLibrary = nil;

function Generate(library, path)
	log.Init("InterfaceMh");
	log.Write = function(self, msg) 			
			-- print("[InterfaceMh]:" .. msg);
		end;
    log:Write( "Generating Macaroni Header File\n");
    targetLibrary = library;
    local gen = MhFileGenerator.new(path, library);
    gen:iterateNodes(library.Context.Root.Children);     
end

function Install(library, sourcePaths, outputPath, installPath, extraArgs)
	local dstPath = installPath:NewPathForceSlash("Mh");
	local interfacePath = outputPath:NewPathForceSlash("Interface.mh");
	interfacePath:CopyToDifferentRootPath(dstPath);
end
