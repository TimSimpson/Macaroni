require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.IO.Path";
--require "Macaroni.IO.PathList";

function boostSystemProperties()
	if (properties.boost ~= nil and properties.boost.current ~= nil) then
		local bprops = properties.boost.current;
		return bprops;
	end
	return nil;
end


function createDependencyList(library)
	local rtn = {};
	local deps = library.Dependencies;
	for k, v in pairs(deps) do
		local dep = v;
		local jamDir = dependencyJamDir(v);
		if (jamDir ~= nil) then
			local element = { };
			element.jamDir = jamDir.AbsolutePath;
			element.name = createProjectName(dep);
			rtn[#rtn + 1] = element; 
		end		
	end	
	return rtn;
end

function Build(library, sources, outputPath, installPath, extraArgs)
	local excludePattern;
	if (extraArgs == nil or extraArgs.ExcludePattern == nil) then
		excludePattern = ".svn";
	else
		excludePattern = extraArgs.ExcludePattern;	
	end
	local extraTargets;
	if (extraArgs == nil or extraArgs.ExtraTargets == nil) then
		extraTargets = "";
	else
		extraTargets = extraArgs.ExtraTargets;	
	end
	createJamroot(library, sources, outputPath, excludePattern, extraTargets);
	local rtnCode = os.execute("bjam link=static " .. outputPath.AbsolutePath)
    print("BJAM return code = " .. rtnCode .. ".")
    if (rtnCode ~= 0) then
        error("Call to Boost.Build failed.")
        return false;
    end    
end


function createJamroot(library, sources, outputPath, excludePattern, extraTargets)
	local buildjam = outputPath:NewPath("/jamroot.jam");
	print("Creating Boost.Build file at " .. buildjam.AbsolutePath .. ".");
	
	local writer = buildjam:CreateFile();		
	
	local forAllSourcesWrite = function(text) 
		-- Because this gets generated to the output path, a relative path will 
		-- work.
		 -- writer:Write(text('./')); -- outputPath.AbsolutePath));
		-- ^- Actually, NO, you can't do this, thanks to yet another undocumented
		-- "feature" of Boost.Build.  If you use "path.glob-tree" with a 
		-- relative path being the source directory, will first off the code 
		-- above is wrong because it generates ".//" which means Boost just
		-- ignores it.  But when I changed it to "./" suddenly Boost started
		-- trying to include targets from the calling project (this occured
		-- when trying to use Lua as a dependency with a generated Jamroot)!
		-- So instead you have to use the vanilla "glob-tree" to avoid
		-- this unexplained behavior.  Of course, maybe this means *ANY* use
		-- of "path.glob-tree" is doomed to failure if its in a project that
		-- is being referenced by another project! Wonderful!!
		-- Spend hours figuring this out.
		for i = 1, #sources do
			local source = sources[i];
			writer:Write(text(source.AbsolutePath));		
		end	
	end;
	
	local pDeps = createDependencyList(library);
	
	writer:Write([[
# Generated by Macaroni.
# ~~ Library Info ~~
# Group   : ]] .. library.Group .. "\n" .. [[
# Name    : ]] .. library.Name .. "\n" .. [[
# Version : ]] .. library.Version .. "\n" .. [[

import boost ;
import path ;
using testing ;

]]);

	for k, v in pairs(pDeps) do
		writer:Write("use-project /" .. v.name .. [[ : "]] .. v.jamDir .. [[" ; ]] .. "\n");
	end
	-- printDependencyProjectIncludes(writer, library);
	writer:Write([[	
project ]] .. createProjectName(library) .. "\n" .. [[
	:	usage-requirements
		<link>shared:<define>]] .. createProjectDef(library) .. [[_DYN_LINK=1
		<link>static:<define>]] .. createProjectDef(library) .. [[_STATIC_LINK=1
]]);
	writer:Write("\t\t<include>./ \n");
	forAllSourcesWrite(function(src) return "\t\t" .. [[<include>"]] .. src .. [["]] .. " \n"; end);
	
	local boostProps = boostSystemProperties();
	if (boostProps ~= nil) then
		writer:Write("\t\t" .. [[<include>"]] .. boostProps.include .. [["]] .. " \n");
	end
	
	for k, v in pairs(pDeps) do
		writer:Write("\t\t<dependency>/" .. v.name .. "//library\n");
	end
	
	writer:Write([[
	;
	
alias library_dependencies
	:	]]);
	for k, v in pairs(pDeps) do
		writer:Write("/" .. v.name .. "//library\n\t\t");
	end
	writer:Write([[ ;
		
alias library_sources
	:	]]);
	writer:Write("[ glob-tree *.c : " .. excludePattern .. " ]\n\t\t"
				.. "[ glob-tree *.cpp : " .. excludePattern .. " ]\n\t\t");				  
	forAllSourcesWrite(function(src) return 
		[[[ path.glob-tree "]] .. src .. [[/" : *.c : ]] .. excludePattern .. " ]\n\t\t" ..
		[[[ path.glob-tree "]] .. src .. [[/" : *.cpp : ]] .. excludePattern .. " ]\n\t\t"; 
		end);
	--for k, v in pairs(library.Dependencies) do
	--	local jamDir = dependencyJamDir(v);
	--	if (jamDir ~= nil) then
	--		writer:Write([["]] .. jamDir.AbsolutePath .. [[//libSources" ]]);
	--	end
	--end
	-- writer:Write(" : ");
	-- writer:Write([[ <include>./ ]]);
	--forAllSourcesWrite(function(src) return [[
	--	<include>"]] .. src .. [["		
	--	]]; end);
	writer:Write(";\n" .. [[
	
lib library
	:	library_dependencies
		library_sources 
	:	<link>shared:<define>]] .. createProjectDef(library) .. [[_DYN_LINK=1
		<link>static:<define>]] .. createProjectDef(library) .. [[_STATIC_LINK=1
	;
	
# Extra targets specified in Macaroni manifest:]] .. "\n");
	-- I don't think I should put this junk in there anymore...
	--for k, v in pairs(library.Dependencies) do
	--	if (dependencyJamDir(v) ~= nil) then
	--		writer:Write([[<use>/]] .. createProjectName(v) .. [[//library ]]);
	--	end
	--end
	--writer:Write([[
	--:	]]);
	--for k, v in pairs(library.Dependencies) do
	--	if (dependencyJamDir(v) ~= nil) then
	--		writer:Write([[<library>/]] .. createProjectName(v) .. [[//library ]]);
	--	end
	--end
	--writer:Write([[
	--;	
    --]]);
    writer:Write(extraTargets);
    writer:Close();    
    
end

function createProjectDef(library)
	return "MACARONI_LIB_" .. createProjectName(library);
end

function createProjectName(library)
	return cleanUpName(library.Group) .. "____" 
		.. cleanUpName(library.Name) .. "____" 
		.. cleanUpName(library.Version);
end

function cleanUpName(str)
	return string.gsub(str, "_", "_u").gsub(str, " ", "_s");	
end

function dependencyJamDir(d)	
	local success, path = pcall(d.FindInstallPath, d);		
	if (success and path ~= nil) then			
		local pathText = path.AbsolutePath;
		local jamroot = path:NewPathForceSlash('Cpp/jamroot.jam');
		if (jamroot.Exists) then
			return path:NewPathForceSlash('Cpp');
		end			
	end
	return nil;
end

function Install(library, sourcePaths, outputPath, installPath, extraArgs)
	-- Create a Jam file which simply points to the source files.
	-- Copy all C++ source to the folder.
	local dstPath = installPath:NewPathForceSlash("Cpp");
	local patterns = {[[\.c(pp)?$]], [[\.h(pp)?$]]}
	local paths = sourcePaths 	
	for k,pattern in ipairs(patterns) do
		for i = 0, #sourcePaths do			
			local path;
			if (i == 0) then 
				path = outputPath; 				
			else
				path = sourcePaths[i];
			end
			copyCppSource(pattern, path, dstPath);				
		end 
	end	
	
	local excludePattern;
	if (extraArgs == nil or extraArgs.ExcludePattern == nil) then
		excludePattern = ".svn";
	else
		excludePattern = extraArgs.ExcludePattern;	
	end
	--local iJam = dstPath:NewPathForceSlash('jamroot.jam');
	--local writer = iJam:CreateFile();
	--writer:Write([[
	--# Generated by Macaroni
	--
	--]]);
	--writer:Close();
	createJamroot(library, {}, dstPath, excludePattern, '')
	return nil; --{ mario = "One good game." };
end

-- Copy all .C, .CPP, .H and .HPP files to dir.=
function copyCppSource(regEx, src, dst) 
	local srcs = src:GetPaths(regEx);	
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

function Prepare(library, sourcePaths, outputPath, installPath, extraArgs)
end

function printDependencyProjectIncludes(writer, library)
	writer:Write([[
# Dependencies, in the form of Boost Build projects. ]]);
	local deps = library.Dependencies;
	writer:Write("# There are " .. #deps .. " dependencies.\n");
	for k, v in pairs(deps) do
		local dep = v;
		local jamDir = dependencyJamDir(v);
		writer:Write("# " .. dep.Group .. " // " .. dep.Name .. "//" .. dep.Version .. "\n");
		if (jamDir == nil) then
			writer:Write("# nil\n");
		else
			writer:Write("use-project /" .. createProjectName(dep)
				.. " : \"" .. jamDir.AbsolutePath .. "\" ;\n");
		end
	end	
end