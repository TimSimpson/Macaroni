require "Macaroni.Model.AnnotationTable"
require "Macaroni.Model.AnnotationValue";
require "Macaroni.Model.Axiom";
require "Macaroni.Model.Block";
require "Macaroni.Model.Cpp.Class";
require "Cpp/Common";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Cpp.Function";
require "Macaroni.IO.GeneratedFileWriter";
require "LuaGlue/LuaGlueCppFile";
require "LuaGlue/LuaGlueHFile";
require "Macaroni.Model.Library";
require "Macaroni.Doc.MDocParser";
if MACARONI_VERSION == "0.1.0.22" then
	require "Macaroni.Model.Member";
else
	require "Macaroni.Model.Element";
end
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.IO.Path";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";
require "Log";
require "io";

Axiom = Macaroni.Model.Axiom;
Block = Macaroni.Model.Axiom;
Class = Macaroni.Model.Cpp.Class;
Context = Macaroni.Model.Context;
FileName = Macaroni.Model.FileName;
Function = Macaroni.Model.Cpp.Function;
MDocParser = Macaroni.Doc.MDocParser;
if MACARONI_VERSION == "0.1.0.22" then
	Member = Macaroni.Model.Member;
else
	--TODO: change Member to Element
	Member = Macaroni.Model.Element;
end
NodeList = Macaroni.Model.NodeList;
Reason = Macaroni.Model.Reason;
Source = Macaroni.Model.Source;
Type = Macaroni.Model.Type;
Variable = Macaroni.Model.Cpp.Variable;

mdocAssociatedFiles = [[\.(mdoc|html|htm|gif|png|jpg|jpeg)?$]];

local check = function(con, msg)
	if (not con) then
		error(msg, 3);
	end
end

local notNull = function(name, value)
	if (value == nil) then
		error('Bad argument:"' .. name .. '" cannot be null.', 3);
	end
end

function Build(library, sources, outputPath, installPath, extraArgs)	
	log.Init("Site");
	local sitePath = outputPath:NewPath("/www/site"):CreateWithCurrentAsRoot();
	log:Write("SITEPATH = " .. sitePath.AbsolutePath);
	sitePath:CreateDirectory();
	local gen = SiteGenerator.new(sitePath, library);
	log:Write("SITEPATH = " .. gen.outputPath.AbsolutePath);
	for i = 1, #sources do
		local s = sources[i];
		gen:iterateDirectory(s);		
	end	
end

function Install(library, sourcePaths, outputPath, installPath, extraArgs)
	log.Init("Site");
	function copyToInstallPath(p)
		local paths = p:GetPaths(mdocAssociatedFiles)
		for i = 1, #paths do
			paths[i]:CopyToDifferentRootPath(installPath, true);
		end
	end
	for i = 1, #sourcePaths do
		copyToInstallPath(sourcePaths[i]);
	end
	copyToInstallPath(outputPath);	
end

function stringEndsWith(str, endsWith)
	local endsWithSize = #endsWith;
	local strSize = #str;
	local suffix = '';
	if (strSize >= endsWithSize) then
		suffix = str:sub(strSize - endsWithSize + 1);		
	else
		suffix = str;		
	end
	return suffix == endsWith;
end

SiteGenerator = 
{
	new = function(outputPath, library)    
		check(outputPath ~= nil, 'Argument 1, "outputPath", must be specified.');
		check(library ~= nil, 'Argument 2, "library", must be specified.');
        local self = {}
        self.outputPath = outputPath;
        self.library = library;
        setmetatable(self, {["__index"] = SiteGenerator});                                 
        return self;
    end,       
    
    lHtmlToHtmlPath = function(self, path)
		local dir = path.ParentPath;
		local oldFileName = path.FileName;
		local newFileName = oldFileName:sub(1, #oldFileName - 4);
		newFileName = newFileName .. 'html';
		return dir:NewPathForceSlash(newFileName);
	end,
	
	iterateDirectory = function(self, inputPath)
		log:Write("self.outputPath=" .. self.outputPath.AbsolutePath);
		if (not inputPath.IsDirectory) then
			self:iterateFile(inputPath);		
		else
			local paths = inputPath.Paths;
			for i = 1, #paths do
				local p = paths[i];
				self:iterateDirectory(p);
			end		
		end
	end,
	
	iterateFile = function(self, inputPath)
		if (self:pathIsLuaHtml(inputPath)) then
			local outputPathL = inputPath:CreateWithDifferentRootPath(self.outputPath);
			local outputPath = self:lHtmlToHtmlPath(outputPathL);
			log:Write("self.outputPath=" .. self.outputPath.AbsolutePath)
			log:Write("in =" .. tostring(inputPath.AbsolutePath));
			log:Write("out=" .. tostring(outputPath.AbsolutePath));
			-- Store vars where script can't find them.
			local sg = SiteGenerator;
			SiteGenerator = nil;
			-- Add vars...
			local oldLibrary = library;
			local oldContext = context;
			library = self.library;
			context = self.library.Context;
			MDocParser.Parse(inputPath, outputPath)
			library = oldLibrary;
			context = oldContext;
			SiteGenerator = sg;
		elseif (self:pathIsCopyResource(inputPath)) then
			inputPath:CopyToDifferentRootPath(self.outputPath, true);
		end		
	end, 		

	pathIsLuaHtml = function(self, path)
		--local str = tostring(path);
		--local size = #str;
		--local ext;
		--if (size < 4) then
	--		ext = str;
		--else
	--		ext = str:sub(size - 4);		
		--end
		--return ext == string.lower(".mdoc");
		return stringEndsWith(tostring(path), ".mdoc");
	end,		
		
	
	pathIsCopyResource = function(self, path)
		local str = tostring(path);
		return stringEndsWith(str, ".css") 
		    or stringEndsWith(str, ".jpg")
			or stringEndsWith(str, ".gif")
			or stringEndsWith(str, ".png");
	end,
	
}




Site = 
-- Helper functions
{
	htmlCodes = {
		{"&", "&amp;"},
		{"<", "&lt;"},
		{">", "&gt;"},
		{" ", "&nbsp;"},
		{'"' ,"&quot;"},
		{"'", "&#39;"},
		{"`", "&lsquo;"},
		{"\n", "<br/>"}
	},
	
	luaToHtml = function(original)
		Site.luaToHtmlBegin();
		Site.textToHtml(original);
		Site.luaToHtmlEnd();
	end,	
	
	luaToHtmlBegin = function(original)
		writer:Write([[<pre class="lua-code">]]);
	end,
	
	luaToHtmlEnd = function(original)
		writer:Write([[</pre>]]);
	end,
	
	pathToLibraryFile = function(libraryId, relativeFilePath)
		check(libraryId.Group ~= nil, "libraryId arg is missing 'Group'");
		check(libraryId.Name ~= nil, "libraryId arg is missing 'Name'");
		check(libraryId.Version ~= nil, "libraryId arg is missing 'Version'");
		function libIdToString(libraryId)
			return "group=" .. libraryId.Group
				   .. ", name=" .. libraryId.Name 
				   .. ", version=" .. libraryId.Version;
		end
		local context = library.Context; -- library is a global var
		local otherLibrary = context:FindLibrary(
			libraryId.Group, libraryId.Name, libraryId.Version);	
		if (otherLibrary == nil) then
			error("Could not find library " .. libIdToString(libraryId));
		end
		
		local installPath = otherLibrary:FindInstallPath();
		if (installPath == nil) then
			error("Could not find install path for library " 
				  .. libIdToString(libraryId)); 
		end
		local realFilePath = installPath:NewPathForceSlash(relativeFilePath);
		if (not realFilePath.Exists) then
			error("Relative file path " .. relativeFilePath .. " was not found "
				  .. " in the installed artifacts for library "
				  .. libIdToString(libraryId) .. " at path " 
				  .. realFilePath.AbsolutePath .. ".");
		end
		return realFilePath;
	end,
	
	readLibraryFile = function(libraryId, relativeFilePath, from, to)
		-- Reads an installed artifact file from a library and returns it as a
		-- string.
		-- libraryId = A table with group, name, and id.
		-- relativeFilePath - Path in terms of the library.
		-- from - The first line to read in (default is 1).
		-- to - The last line to read (default is -1 for EOF).
		from = from or 1
		to = to or -1
		local realFilePath = Site.pathToLibraryFile(libraryId, 
		                                            relativeFilePath);
		local lines = {}
		local number = 1
		for line in io.lines(realFilePath.AbsolutePath) do
			if (number >= from and to < 0 or number <= to) then
				table.insert(lines, line)
			end
			number = number + 1
		end
		return table.concat(lines, "\n");		
	end,
	
	textToHtml = function(original)
		check(writer ~= nil, "Global variable 'writer' is nil! You are " ..
		                     "calling this from within an mdoc, right?")
		modified = original
		for i, v in ipairs(Site.htmlCodes) do
			modified = modified:gsub(v[1], v[2])
		end		
		writer:Write(modified)
	end,
	
	shellToHtml = function(original)
		writer:Write([[<pre class="shell">]]);
		Site.textToHtml(original);
		writer:Write([[</pre>]]);
	end,
		
}; 
