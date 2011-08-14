require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";

function getIncludePath()
	local success, path = pcall(function()
		return properties.boost["1.46.1"].include;
	end);	
	if (not success) then
		error([[Could not find variable properties.boost["1.45.0"].include.]]
			.. [[  Please set this in init.lua to the path of Boost headers.]]);
	end
	return path;
end

function Prepare(library, sources, outputPath, installPath, extraArgs)
	local includePath = getIncludePath();	
	local buildjam = outputPath:NewPath("/jamroot.jam");
	print("Creating Boost.Build file at " .. buildjam.AbsolutePath .. ".");
	local writer = buildjam:CreateFile();			
	
	writer:Write([[
# Generated by Macaroni.

import boost ;
import path ;

use-project /boost : "]] .. includePath .. [[" ;

alias library
	: /boost//headers
	;
	
]]);
    writer:Close();    
end
