require "Macaroni.IO.Path";
Path = Macaroni.IO.Path;

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.Release",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Bundles Macaroni distribution files and the creates the Macaroni website.
]]

dependency {group=upper.Group, name="Macaroni.Tests.Bugs", version=upper.Version}

sources = { "source/scripts", "source/www" }
output = "target"
   	
local manifestDir = Path.New(manifestDirectory)
function newPath(suffix)
	return manifestDir:NewPathForceSlash(suffix):CreateWithCurrentAsRoot();	
end	

function copyCppFiles(src, dst)
	copyFiles(src, dst, [[\.(c|cpp|h|hpp)?$]]);
end

function copyFiles(src, dst, regex)
	local filePaths = src:GetPaths(regex)
	for i = 1, #filePaths do
		local fp = filePaths[i];
		fp:CopyToDifferentRootPath(dst, true);
		print(tostring(fp));
	end
end

function copyJamFiles(src, dst)
	copyFiles(src, dst, [[\.jam?$]]);
end

function copyResourceFiles(src, dst)
	copyFiles(src, dst, [[\.(ico|rc|rc2|h)?$]]);
end

function createDistributionDirectory()
	local dstDir = newPath("target/macaroni-" .. upper.Version);
	newPath("../Main/App/GeneratedSource/release/debug"):NewPath("/macaroni.exe")
		:CopyToDifferentRootPath(dstDir, true);
	newPath("../Main/App/GeneratedSource/release/debug"):NewPath("/messages.txt")
		:CopyToDifferentRootPath(dstDir, true);
	copyFiles(newPath("../Main"):NewPath("/Generators"), dstDir, [[\.lua?$]]);
	copyFiles(newPath("../Main"):NewPath("/Libraries"), dstDir, [[\.(jam|lua|mh)?$]]);	
end

function createPureCpp()	
	local dstDir = newPath("target/macaroni-pureCpp-" .. upper.Version);
	copyCppFiles(newPath("../Main/App/Source/main/mcpp"), dstDir);
	copyResourceFiles(newPath("../Main/App/Source/main/resources"), dstDir);
	copyCppFiles(newPath("../Main/App/GeneratedSource"), dstDir);	
	copyJamFiles(newPath("../Main/App/Source/main/pureCppExtraFiles"), dstDir);
end

function build()	
	createDistributionDirectory();
	createPureCpp();
	run("Site");
	local versionDownloads = manifestDir:NewPathForceSlash(
		"target/www/site/downloads/" .. upper.Version);
	versionDownloads:CreateDirectory();
	print("TODO: Zip up distirubiton directory and pure CPP directory and place in "
		.. tostring(versionDownloads) .. '.');
	print("TODO: Zip up the directory " .. tostring(versionDownloads) 
		.. " - it is your release artifact.");
end
