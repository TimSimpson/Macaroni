--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
--
--  This file copies the Lua source code from some a path specified at
--  properties.lua["5.2.3"] to the target directory, in the process changing
-- .c files to .cpp to easily compile it as C++ under VC++.
--
--------------------------------------------------------------------------------

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
		return properties.lua["5.2.3"].include;
	end);
	if (not success) then
		error([[
Could not find the variable properties.lua["5.2.3"].include.
This can be set in the file "init.lua" located in the Macaroni directory under
your home directory.
The variable to set should look something like this:
lua =
  {
    ["5.2.3"] =
    {
      bin = "C:/lua-5.2.3/bin",
      include = "C:/lua-5.2.3/src",
      source = "C:/lua-5.2.3/src"
    },
  },
		]]);
	end
	return path;
end

function Generate(library, path)
  if path.Exists then
    path:ClearDirectoryContents()
  end
end

function Build(library, sources, outputPath, installPath, extraArgs)
	local includePath = getIncludePath();
	copyCFilesToCppFiles(Macaroni.IO.Path.New(includePath), outputPath);
end

