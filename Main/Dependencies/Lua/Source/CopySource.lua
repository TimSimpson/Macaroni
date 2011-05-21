require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function copyCFilesToCppFiles(src, dst)		
	local srcFiles = src:GetPaths([[\.(c|h)?$]])
	for i = 1, #srcFiles do
		local fp = srcFiles[i];
		fp:CopyToDifferentRootPath(dst, true);
	end
	local dstFiles = dst:GetPaths([[\.(c)?$]])
	for j = 1, #dstFiles do		
		local fp = dstFiles[j];
		local rp = string.gsub(fp.RelativePath, ".c", ".cpp")
		fp:RenameRelative(rp)
	end		
end

function getIncludePath()	
	local success, path = pcall(function()
		return properties.lua["5.1.4"].include;
	end);
	if (not success) then
		error([[
Could not find the variable properties.lua["5.1.4"].include.
This can be set in the file "init.lua" located in the Macaroni directory under
your home directory.
The variable to set should look something like this:
lua =
  {
    ["5.1.4"] =
    {
      bin = "C:/lua-5.1.4/bin",      
      include = "C:/lua-5.1.4/src",
      source = "C:/lua-5.1.4/src"
    },
  },
		]]);
	end
	return path;
end

function Generate(library, path)
	path:ClearDirectoryContents()
end

function Build(library, sources, outputPath, installPath, extraArgs)
	local includePath = getIncludePath();	
	copyCFilesToCppFiles(Macaroni.IO.Path.New(includePath), outputPath);	
end

