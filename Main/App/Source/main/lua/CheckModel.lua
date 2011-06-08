require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function Generate(library, path)
	local root = library.Context.Root;
	local cpp = root:FindOrCreate("Macaroni::Model::Cpp::ClassParentPtr")
	if cpp.Member == nil or cpp.Member.TypeName ~= "Typedef" then
		error("NO!  Was " .. cpp.Member.TypeName);
	end
	if cpp.HFilePath == nil then
		error("NO!GG");
	end
	-- error("" .. tostring(cpp.HFilePath));
end
