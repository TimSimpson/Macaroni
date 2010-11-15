require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function Generate(library, path)		
	printDependencies(library);
end

function printDependencies(library)
	print("Dependencies of " .. tostring(library))
	for k, v in pairs(library.Dependencies) do
		local success, path = pcall(v.FindInstallPath, v);
		local pathText;
		if (success == false) then
			pathText = path;
		else
			pathText = path.AbsolutePath;
		end
		print('\t' .. tostring(k) .. '=' .. tostring(v) .. '\t' .. pathText);
	end
end

