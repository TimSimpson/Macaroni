require "Macaroni.Model.AnnotationTable"
require "Macaroni.Model.AnnotationValue";
local Axiom = require "Macaroni.Model.Axiom";
local Block = require "Macaroni.Model.Block";
local Class = require "Macaroni.Model.Cpp.Class";
require "Cpp/Common";
local Context = require "Macaroni.Model.Context";
local FileName = require "Macaroni.Model.FileName";
local Function = require "Macaroni.Model.Cpp.Function";
require "Macaroni.IO.GeneratedFileWriter";
require "LuaGlue/LuaGlueCppFile";
require "LuaGlue/LuaGlueHFile";
local MDocParser = require "Macaroni.Doc.MDocParser";
local Node = require "Macaroni.Model.Node";
local NodeList = require "Macaroni.Model.NodeList";
local Path = require "Macaroni.IO.Path";
local Reason = require "Macaroni.Model.Reason";
local Source = require "Macaroni.Model.Source";
local Type = require "Macaroni.Model.Type";
local Variable = require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";
require "Log";
require "io";
require "string";


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

function Build(project, sources, outputPath, output,
	           extension, outputSubPath)
	log.Init("Site");
	local outputSubPath = outputSubPath or "/www/site"
	local sitePath = outputPath:NewPath(outputSubPath):CreateWithCurrentAsRoot();
	log:Write("SITEPATH = " .. sitePath.AbsolutePath);
	sitePath:CreateDirectory();
	local gen = SiteGenerator.new(sitePath, project, output,
		extension);
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

function GetMethod(name)
    if name == "build" then
        return
        {
            Describe = function(args)
                args.output.WriteLine("Generate site.")
            end,
            Run = function(args)
            	local projectVersion = args.project;
                local sources = args.sources;
                local outputPath = args.outputPath;
                local output = args.output;
                context = args.context
                Build(projectVersion, sources, outputPath, output,
                	  args.extension, args.outputSubPath);
            end
        }
    end
    return nil;
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
	new = function(outputPath, project, output, extension)
		check(outputPath ~= nil, 'Argument 1, "outputPath", must be specified.');
		check(project ~= nil, 'Argument 2, "project", must be specified.');
        local self = {}
        self.outputPath = outputPath;
        self.output = output;
        self.project = project;
        self.extension = extension or "html"
        setmetatable(self, {["__index"] = SiteGenerator});
        return self;
    end,

    lHtmlToHtmlPath = function(self, path)
		local dir = path.ParentPath;
		local oldFileName = path.FileName;
		local newFileName = oldFileName:sub(1, #oldFileName - 4);
		newFileName = newFileName .. self.extension;
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
			local oldProject = project;
			project = self.project;
			local status, err = pcall(function()
				MDocParser.Parse(inputPath, outputPath)
			end)
			if not status then
				self.output:WriteLine("Error parsing " .. tostring(inputPath)
					                  .. ":")
				self.output:Error(tostring(err))
				error(err)
			end
			project = oldProject;
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
			or stringEndsWith(str, ".png")
			or stringEndsWith(str, ".py");
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

	codeFileToHtml = function(filePath, language)
		Site.luaToHtmlBegin();

		cmd = "pygmentize -f html -l " .. language
		      .. " " .. filePath.AbsolutePath
		prog = io.popen(cmd, 'r');
		line = prog:read()
		while line ~= nil do
			line = string.gsub(line,
				"<pre>",
				[[<pre style="white-space: pre-wrap; /* css-3 */
	white-space: -moz-pre-wrap !important; /* Mozilla, since 1999 */
	white-space: -pre-wrap; /* Opera 4-6 */
	white-space: -o-pre-wrap; /* Opera 7 */
	word-wrap: break-word; /* Internet Explorer 5.5+ */"
	class="code-block-bg">]])
			writer:WriteLine(line);
			line = prog:read()
		end

		Site.luaToHtmlEnd();
	end,

	cppFileToHtml = function(filePath)
		Site.codeFileToHtml(filePath, "cpp")
	end,

	mcppFileToHtml = function(filePath)
		Site.codeFileToHtml(filePath, "cpp")
	end,

	luaFileToHtml = function(filePath)
		Site.codeFileToHtml(filePath, "lua")
	end,

	luaToHtml = function(original)
		Site.luaToHtmlBegin();
		Site.textToHtml(original);
		Site.luaToHtmlEnd();
	end,

	luaToHtmlBegin = function(original)
		writer:Write([[<pre class="code">]]);
	end,

	luaToHtmlEnd = function(original)
		writer:Write([[</pre>]]);
	end,

	pathToProjectFile = function(projectVersionId, relativeFilePath)
		check(projectVersionId.Group ~= nil, "libraryId arg is missing 'Group'");
		check(projectVersionId.Name ~= nil, "libraryId arg is missing 'Name'");
		check(projectVersionId.Version ~= nil, "libraryId arg is missing 'Version'");
		function pvIdToString(projectVersionId)
			return "group=" .. projectVersionId.Group
				   .. ", name=" .. projectVersionId.Name
				   .. ", version=" .. projectVersionId.Version;
		end
		local otherProject = context:FindProjectVersion(
			projectVersionId.Group,
			projectVersionId.Name,
			projectVersionId.Version);
		if (otherProject == nil) then
			error("Could not find project " .. pvIdToString(projectVersionId));
		end

		local installPath = findInstallPath(otherProject);
		if (installPath == nil) then
			error("Could not find install path for library "
				  .. pvIdToString(projectVersionId));
		end
		local realFilePath = installPath:NewPathForceSlash(relativeFilePath);
		if (not realFilePath.Exists) then
			error("Relative file path " .. relativeFilePath .. " was not found "
				  .. " in the installed artifacts for library "
				  .. pvIdToString(projectVersionId) .. " at path "
				  .. realFilePath.AbsolutePath .. ".");
		end
		return realFilePath;
	end,

	readLibraryFile = function(libraryId, relativeFilePath, from, to)
		error("This function is now called 'readProjectFile.'")
	end,

	readProjectFile = function(projectVersionId, relativeFilePath, from, to)
		-- Reads an installed artifact file from a library and returns it as a
		-- string.
		-- libraryId = A table with group, name, and id.
		-- relativeFilePath - Path in terms of the library.
		-- from - The first line to read in (default is 1).
		-- to - The last line to read (default is -1 for EOF).
		from = from or 1
		to = to or -1
		local realFilePath = Site.pathToProjectFile(projectVersionId,
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
