require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function Build(library, sources, outputPath, installPath, extraArgs)
	local libraryPath = sources[#sources];
	local outputLibraryPath = outputPath:NewPathForceSlash("Libraries");
	if (not outputLibraryPath.Exists) then
		copyDirectory(libraryPath, outputLibraryPath);
	end
  --	local rootPath = outputPath:NewPath("/../..");
 --local exePath = rootPath:NewPath("/App/GeneratedSource/release/debug/macaroni.exe");
  --print("EXEPATH   ==" .. tostring(exePath));
  --print("ROOTPATH  ==" .. tostring(rootPath));
  --print("outputPath==" .. tostring(outputPath.AbsolutePath));
  --exePath:CopyToDifferentRootPath(outputPath:NewPath("./"));
  --local generatorsPath = rootPath:NewPath("/Generators");
  --generatorsPath:CopyToDifferentRootPath(outputPath);
  --local libraryPath = rootPath:NewPath("/Library");
  --libraryPath:CopyToDifferentRootPath(outputPath);
end


-- Copy all .C, .CPP, .H and .HPP files to dir.=
function copyDirectory(src, dst)
	local srcs = src:GetPaths("");
	for i = 1, #srcs do
		local child = srcs[i];
		if (not child.IsDirectory) then
			print(tostring(child.AbsolutePath) .. " ... " .. tostring(dst.AbsolutePath));
			--src:CreateDirectory();
			child:CopyToDifferentRootPath(dst);
		else
			-- May not be necessary
			-- iterateDir(regEx, child, dst);
		end
	end
end
