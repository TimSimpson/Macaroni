-- This generator saves a copy of all files to the install directory.
-- If the multiple outdifferent source directories or 
require "Cpp/Common";
require "Cpp/CppFileGenerator";
require "Cpp/HFileGenerator";
require "Macaroni.Model.Library";
require "Macaroni/MhFileGenerator";
require "Log";
require "Macaroni.IO.Path";

Path = Macaroni.IO.Path;

function Install(library, sourcePaths, outputPath, installPath, extraArgs)
	if (extraArgs.directory == nil) then
		error("Missing 'extraArg' value 'directory'. This value must be "
		      .. "set to a string that is the directory to be archived.");
	end
	if (extraArgs.regex == nil) then
		error("Missing 'extraArg' value 'regex'. This value must be "
		      .. "set to a regular expression string of which files to "
		      .. "archive.");
	end
	local dir = Path.New(extraArgs.directory);	
	copySourceToInstallPath(dir, installPath, extraArgs.regex);	
end

function copySourceToInstallPath(sourcePath, installPath, regex)	
	local dstPath = installPath:NewPathForceSlash("original");
	local filePaths = sourcePath:GetPaths(regex)
	for i = 1, #filePaths do
		local fp = filePaths[i];
		local fp = filePaths[i];
		fp:CopyToDifferentRootPath(dstPath, true);
	end	
end