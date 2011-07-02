require "Macaroni.IO.GeneratedFileWriter"
require "Macaroni.Model.Library";
require "Macaroni.IO.Path"

Path = Macaroni.IO.Path

function checkArgs(args, name, description)
	if args[name] == nil then
		msg = 'Missing argument "' .. name;
		if description ~= nil then
			msg = msg .. " Description:" .. description;
		end
		error(msg)
	end
end

function Build(library, sources, outputPath, installPath, extraArgs)
	-- Creates a Visual C++ project.	
	local proj = ProjectFileWriter.new(extraArgs, library, sources, outputPath)
	proj:WriteFile()	
end

ProjectFileWriter = {
	new = function(args, library, sources, outputPath)
		if args == nil then error("Must include extraArgs.") end
		checkArgs(args, "ProjectFile", 
		          "The path to put the generated project file.");
		checkArgs(args, "ProjectGUID");
		checkArgs(args, "RootNamespace");
		checkArgs(args, "PlatformNames");
		if args.Name == nil then
			args.Name = library.Name
		end
		if args.Configurations == nil then
			args.Configurations = {
				{ 
				  Name="Debug|Win32", 
				  PreprocessorDefinitions="WIN32;_DEBUG;_CONSOLE"
				 }
			};
		end
		args.library = library
		args.sources = sources
		args.outputPath = outputPath
		self = args
		self.writer =  Path.New(self.ProjectFile):CreateFile()        
		setmetatable(self, {["__index"]=ProjectFileWriter});
		return self		
	end,
	
	WriteFile = function(self)
		self:writeBegin();
		self:writePlatforms();
		self:writeConfigurations();
		self.writer:Write([[
		<References>
		</References>
		]]);
		self:writeFiles();
		self.writer:Write([[
		<Globals>
		</Globals>
		]]);		
		self:writeEnd();
	end,
	
	writeBegin = function(self)
		self.writer:Write([[<?xml version="1.0" encoding="Windows-1252"?>
	<VisualStudioProject
		ProjectType="Visual C++"
		Version="9.00"
		Name="]] .. self.Name .. [["
		ProjectGUID="{]] .. self.ProjectGUID .. [[}"
		RootNamespace="]] .. self.RootNamespace .. [["
		Keyword="Win32Proj"
		TargetFrameworkVersion="196613"
		>
		]]);
	end,
	
	writeConfiguration = function(self, config)
		self.writer:Write([[<Configuration
			Name="]] .. config.Name .. [["
			OutputDirectory="$(SolutionDir)$(ConfigurationName)"
			IntermediateDirectory="$(ConfigurationName)"
			ConfigurationType="1"
			CharacterSet="1"
			>
			<Tool
				Name="VCPreBuildEventTool"
				CommandLine="macaroni.exe generate"
			/>
			<Tool
				Name="VCCLCompilerTool"
				Optimization="0"
				AdditionalIncludeDirectories="]] .. 
					self:writeIncludeDirectories(";") ..
				[["
				PreprocessorDefinitions="]] .. 
					config.PreprocessorDefinitions .. 
				[["
				MinimalRebuild="true"
				BasicRuntimeChecks="3"
				RuntimeLibrary="3"
				UsePrecompiledHeader="0"
				WarningLevel="3"
				DebugInformationFormat="4"
			/>
			<Tool
				Name="VCLinkerTool"
				LinkIncremental="2"
				AdditionalLibraryDirectories="]] .. 
				self:writeLibraryDirectories(';') ..
				[["
				GenerateDebugInformation="true"
				SubSystem="1"
				TargetMachine="1"
			/>
			</Configuration>
			]]);
	end,
	
	writeConfigurations = function(self)
		self.writer:Write("<Configurations>");
		for i, v in ipairs(self.Configurations) do
			self:writeConfiguration(v);
		end
		self.writer:Write("</Configurations>");		
	end,	

	
	writeDirFiles = function(self, src, regex)
		local filePaths = src:GetPaths(regex)
		for i = 1, #filePaths do
			local fp = filePaths[i];
			self.writer:Write([[<File RelativePath="]])
			self.writer:Write(fp.AbsolutePath .. [[" />]] .. '\n')
		end
	end	,
	
	writeFiles = function(self)
		self.writer:Write("<Files>");
		for i, v in ipairs(self.sources) do
			self:writeDirFiles(v, [[\.(c|cpp|h|hpp|mcpp)?$]])
		end
		self:writeDirFiles(self.outputPath, [[\.(c|cpp)?$]])
		self.writer:Write("</Files>");
	end,
	
	writeEnd = function(self)
		self.writer:Write("\n" .. [[</VisualStudioProject>]]);
		self.writer:Close();
	end,
	
	writeIncludeDirectories = function(self, seperator)
		local rtn = ""
		for i, v in ipairs(self.sources) do
			rtn = rtn .. v.AbsolutePath .. seperator
		end
		rtn = rtn .. self.outputPath.AbsolutePath .. seperator
		for i, v in ipairs(self.library.Dependencies) do
			local dir = guessIncludeLocation(v)
			if dir ~= nil then
				rtn = rtn .. dir .. seperator
			end
		end
		return rtn;
	end,
	
	writeLibraryDirectories = function(self, seperator)
		rtn = ""
		for i, v in ipairs(self.sources) do
			rtn = rtn .. v.AbsolutePath .. seperator
		end
		rtn = rtn .. self.outputPath.AbsolutePath .. seperator
		for i, v in ipairs(self.library.Dependencies) do
			local dir = guessBoostBuildLibraryLocation(v)
			if dir ~= nil then
				rtn = rtn .. dir .. seperator
			end
		end
		return rtn;
	end,
	
	writePlatforms = function(self)
		self.writer:Write("<Platforms>")
		for i, n in ipairs(self.PlatformNames) do
			self.writer:Write('<Platform Name="' .. n .. '" />')
		end
		self.writer:Write("</Platforms>")
	end,
};

function guessIncludeLocation(d)
	-- Given a dependency library, estimates where the included source is.
	local success, path = pcall(d.FindInstallPath, d);		
	if (success and path ~= nil) then			
		local pathText = path.AbsolutePathForceSlash;
		local includeDir = path:NewPathForceSlash('Cpp');
		if (includeDir.Exists) then
			return includeDir.AbsolutePath;
		end			
	end
	return nil;
end

function guessBoostBuildLibraryLocation(d)
	-- Given a dependency library, estimates where Boost Build might have stored
	-- its library files.
	-- Clearly this is an insane hack, because a better solution would have to 
	-- solve for the multitude of possible configurations in a C++ project
	-- and where they all might live.
	local success, path = pcall(d.FindInstallPath, d);		
	if (success and path ~= nil) then			
		local pathText = path.AbsolutePathForceSlash;
		-- local link_static = "Cpp/bin/msvc-9.0/debug/link-static"
		local libDir = path:NewPathForceSlash('Cpp/bin/msvc-9.0/debug/threading-multi');
		if (libDir.Exists) then
			return libDir.AbsolutePath;
		end			
	end
	return nil;
end