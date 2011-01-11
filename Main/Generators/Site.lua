require "Macaroni.Model.AttributeTable"
require "Macaroni.Model.AttributeValue";
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
require "Macaroni.Model.Member";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.IO.Path";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";
require "Macaroni.Model.Type";
require "Macaroni.Model.Cpp.Variable";
require "Cpp/NodeInfo";
require "Log";

Axiom = Macaroni.Model.Axiom;
Block = Macaroni.Model.Axiom;
Class = Macaroni.Model.Cpp.Class;
Context = Macaroni.Model.Context;
FileName = Macaroni.Model.FileName;
Function = Macaroni.Model.Cpp.Function;
MDocParser = Macaroni.Doc.MDocParser;
Member = Macaroni.Model.Member;
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
	
	pathIsCopyResource = function(self, path)
		local str = tostring(path);
		return stringEndsWith(str, ".css") 
			or stringEndsWith(str, ".gif")
			or stringEndsWith(str, ".png");
	end,
	
}



