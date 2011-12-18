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

function Generate(library, sources, outputPath)
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
        if args.Name == nil then
            args.Name = library.Name
        end
        checkArgs(args, "AdditionalIncludePaths");
        checkArgs(args, "AdditionalLibraryPaths");
        checkArgs(args, "SourcePaths");
        if args.AdditionalIncludePaths == nil then
            args.AdditionalIncludePaths = {}
        end
        if args.AdditionalLibraryPaths == nil then
            args.AdditionalLibraryPaths = {}
        end
        if args.AdditionalSourcePaths == nil then
            args.AdditionalSourcePaths = {}
        end
        if args.SourceExcludePatterns == nil then
            args.SourceExcludePatterns = {}
        end
        if args.PreBuildEvent == nil then
            if args.Cavatappi then
              args.PreBuildEvent = "cavatappi generate";
            else
              args.PreBuildEvent = "macaroni.exe generate";
            end
        end
        if args.Configurations == nil then
            args.Configurations = {
                {
                  Name="Debug|Win32",
                  Configuration="Debug",
                  Platform="Win32",
                  PreprocessorDefinitions="WIN32;_DEBUG;_CONSOLE"
                 }
            };
        end
        args.library = library
        args.sources = {}
        for i, v in ipairs(args.SourcePaths) do
            args.sources[#args.sources + 1] = Path.New(v)
        end
        for i, v in ipairs(args.AdditionalSourcePaths) do
            args.sources[#args.sources + 1] = Path.New(v)
        end
        args.outputPath = outputPath
        self = args
        self.writer =  Path.New(self.ProjectFile):CreateFile()
        setmetatable(self, {["__index"]=ProjectFileWriter});
        return self
    end,

    -- function librarySourcePath(d)
    --     local success, path = pcall(d.FindInstallPath, d);
    --     if (success and path ~= nil) then
    --         local pathText = path.AbsolutePathForceSlash;
    --         local sourceFolder = path:NewPathForceSlash('Cpp');
    --         if (sourceFolder.Exists) then
    --             return sourceFolder;
    --         end
    --     end
    --     return nil;
    -- end,

    WriteFile = function(self)
        self.writer:Write([[
<?xml version="1.0" encoding="Windows-1252"?>
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
        self:writePlatforms();
        self.writer:Write([[
        <ToolFiles>
        </ToolFiles>
        ]]);
        self:writeConfigurations();
        self.writer:Write([[
          <References>
          </References>
          ]]);
        self:writeFiles();
        self.writer:Write([[
          <Globals>
          </Globals>
</VisualStudioProject>]]);
    end,

    writeBegin = function(self)
        self.writer:Write([[
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
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
          ]]);
        self.writer:Write([[ CommandLine="]] .. self.PreBuildEvent .. [[" ]]);
        self.writer:Write([[
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
        self.writer:Write([[
            <Configurations>
            ]]);
        for i, v in ipairs(self.Configurations) do
            self:writeConfiguration(v);
        end
        self.writer:Write([[
            </Configurations>
        ]]);
    end,

    filePathExclude = function(self, fp)
      -- Returns true if the given file path should be excluded.
      for i, v in ipairs(self.SourceExcludePatterns) do
          if string.sub(fp.AbsolutePath, - string.len(v)) == v then
              return true
          end
      end
      return false
    end,

    writeFiles = function(self)
        self.writer:Write("<Files>");
        local writeDirFiles = function(src, regex, printFunction)
            local filePaths = src:GetPaths(regex)
            for i = 1, #filePaths do
                local fp = filePaths[i];
                if not self:filePathExclude(fp) then
                  printFunction(fp.AbsolutePath);
                  -- if elementName == "None" then
                  --     self.writer:Write([[<None Include="]] ..
                  --         fp.AbsolutePath ..
                  --         [[">
                  --           <FileType>Document</FileType>
                  --         </None>]])
                  -- else
                  --     self.writer:Write([[<]] .. elementName ..[[ Include="]])
                  --     self.writer:Write(fp.AbsolutePath .. [[" />]] .. '\n')
                  -- end
                end
            end
        end

        -- All the "custom files" don't nest inside an XML element.
        for i, v in ipairs(self.sources) do
            writeDirFiles(v, [[\.(mcpp)?$]], function(fp)
              self.writer:Write([[
      <File RelativePath="]] .. fp .. [[">
        <FileConfiguration
         Name="Debug|Win32"
         ExcludedFromBuild="true">
           <Tool Name="VCCustomBuildTool" />
        </FileConfiguration>
      </File>
              ]]);
            end);
        end

        self.writer:Write([[
        <Filter
          Name="Source Files"
          Filter="cpp;c;cc;cxx;def;odl;idl;hpj;bat;asm;asmx"
          UniqueIdentifier="{4FC737F1-C7A5-4376-A066-2A32D752A2FF}"
          >]]);

        local writeFile = function(fp)
            self.writer:Write([[
                <File RelativePath="]] .. fp .. [[">
                </File>
            ]]);
        end
        for i, v in ipairs(self.sources) do
            writeDirFiles(v, [[\.(c|cc|cpp)?$]], writeFile);
        end
        writeDirFiles(self.outputPath, [[\.(c|cc|cpp)?$]], writeFile);
        self.writer:Write([[</Filter>]]);

        self.writer:Write([[
        <Filter
          Name="Header Files"
          Filter="h;hpp;hxx;hm;inl;inc;xsd"
          UniqueIdentifier="{93995380-89BD-4b04-88EB-625FBE52EBFB}"
          >]]);
        for i, v in ipairs(self.sources) do
            writeDirFiles(v, [[\.(h|hpp)?$]], writeFile);
        end
        writeDirFiles(self.outputPath, [[\.(h|hpp)?$]], writeFile);
        self.writer:Write([[</Filter>]]);

        self.writer:Write("</Files>");
    end,

    writeEnd = function(self)
        self.writer:Write("\n" .. [[
        </Project>
        ]]);
        self.writer:Close();
    end,

    writeIncludeDirectories = function(self, seperator)
        local rtn = ""
        for i, v in ipairs(self.sources) do
            rtn = rtn .. v.AbsolutePath .. seperator
        end
        rtn = rtn .. self.outputPath.AbsolutePath .. seperator
        local appendIncludeDir = function(v)
            local dir = guessIncludeLocation(v)
            if dir ~= nil then
                rtn = rtn .. dir .. seperator
            end
        end
        for i, v in ipairs(self.library.Dependencies) do
            appendIncludeDir(v);
        end
        for i, v in ipairs(self.AdditionalIncludePaths) do
            rtn = rtn .. v .. seperator
        end
        return rtn;
    end,

    writeLibraryDirectories = function(self, seperator)
        local rtn = ""
        for i, v in ipairs(self.sources) do
            rtn = rtn .. v.AbsolutePath .. seperator
        end
        rtn = rtn .. self.outputPath.AbsolutePath .. seperator
        local appendLibPath = function(v)
            local dir = guessBoostBuildLibraryLocation(v)
            if dir ~= nil then
                rtn = rtn .. dir .. seperator
            end
        end
        for i, v in ipairs(self.library.Dependencies) do
            appendLibPath(v);
        end
        for i, v in ipairs(self.AdditionalLibraryPaths) do
            rtn = rtn .. v .. seperator
        end
        return rtn;
    end,

    writePlatforms = function(self)
        self.writer:Write("<Platforms>")
        for i, n in ipairs(self.Configurations) do
            self.writer:Write('<Platform Name="' .. n.Platform .. '" />')
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
