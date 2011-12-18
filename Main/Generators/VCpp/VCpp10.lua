require "Macaroni.IO.GeneratedFileWriter"
require "Macaroni.Model.Library";
require "Macaroni.IO.Path"

Path = Macaroni.IO.Path

function mario(m)
    print("MARIO " .. m)
end

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
    mario("0")
    local proj = ProjectFileWriter.new(extraArgs, library, sources, outputPath)
    mario("2")
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
        if args.Configurations == nil then
            args.Configurations = {
                {
                  Name="Debug|Win32",
                  Configuration="Debug",
                  Platform="Win32",
                  --PreprocessorDefinitions="WIN32;_DEBUG;_CONSOLE"
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
        mario("C")
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
        self:writeBegin();
        self:writeConfigurations();
        self.writer:Write([[
      <PropertyGroup Label="Globals">
        <ProjectGuid>{]] .. self.ProjectGUID .. [[}</ProjectGuid>
        <Keyword>Win32Proj</Keyword>
        <RootNamespace>]] .. self.RootNamespace .. [[</RootNamespace>
      </PropertyGroup>

        ]])

        self.writer:Write([[
<Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>true</UseDebugLibraries>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'" Label="Configuration">
    <ConfigurationType>Application</ConfigurationType>
    <UseDebugLibraries>false</UseDebugLibraries>
    <WholeProgramOptimization>true</WholeProgramOptimization>
    <CharacterSet>Unicode</CharacterSet>
  </PropertyGroup>
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />
  <ImportGroup Label="ExtensionSettings">
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <ImportGroup Label="PropertySheets" Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <Import Project="$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props" Condition="exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')" Label="LocalAppDataPlatform" />
  </ImportGroup>
  <PropertyGroup Label="UserMacros" />
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <LinkIncremental>true</LinkIncremental>
  </PropertyGroup>
  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <LinkIncremental>false</LinkIncremental>
  </PropertyGroup>
  ]]);

    self.writer:Write([[
    <PropertyGroup>
        <LinkIncremental>true</LinkIncremental>
        <IncludePath>]]);

    -- for i, source in ipairs(self.sources) do
    --     self.writer:Write('"' .. source.AbsolutePath .. '";')
    -- end
    -- self.writer:Write('"' .. self.outputPath.AbsolutePath .. '";')
    self.writer:Write([[$(IncludePath)</IncludePath>
      </PropertyGroup>
    ]])

    self.writer:Write([[
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">
    <ClCompile>
      <PrecompiledHeader>
      </PrecompiledHeader>
      <WarningLevel>Level3</WarningLevel>
      <Optimization>Disabled</Optimization>
      <PreprocessorDefinitions>WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
      <AdditionalIncludeDirectories>]]);


    for i, source in ipairs(self.sources) do
        self.writer:Write('"' .. source.AbsolutePath .. '";')
    end
    self.writer:Write('"' .. self.outputPath.AbsolutePath .. '";')
    for i, dir in ipairs(self.AdditionalIncludePaths) do
        self.writer:Write('"' .. Path.New(dir).AbsolutePath .. '";')
    end

      self.writer:Write([[%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <AdditionalLibraryDirectories>]]);

      for i, dir in ipairs(self.AdditionalLibraryPaths) do
          self.writer:Write('"' .. Path.New(dir).AbsolutePath .. '";')
      end

      self.writer:Write([[%(AdditionalLibraryDirectories)
      </AdditionalLibraryDirectories>
    </Link>
  </ItemDefinitionGroup>
  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">
    <ClCompile>
      <WarningLevel>Level3</WarningLevel>
      <PrecompiledHeader>
      </PrecompiledHeader>
      <Optimization>MaxSpeed</Optimization>
      <FunctionLevelLinking>true</FunctionLevelLinking>
      <IntrinsicFunctions>true</IntrinsicFunctions>
      <PreprocessorDefinitions>WIN32;NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    </ClCompile>
    <Link>
      <SubSystem>Console</SubSystem>
      <GenerateDebugInformation>true</GenerateDebugInformation>
      <EnableCOMDATFolding>true</EnableCOMDATFolding>
      <OptimizeReferences>true</OptimizeReferences>
    </Link>
  </ItemDefinitionGroup>
  <ItemGroup>
    <None Include="ReadMe.txt" />
  </ItemGroup>
  ]]);
        self:writeFiles();
        self.writer:Write([[
  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />
  <ImportGroup Label="ExtensionTargets">
  </ImportGroup>
        ]]);

        -- TODO: References?

--        self:writeFiles();


        self:writeEnd();
    end,

    writeBegin = function(self)
        self.writer:Write([[
<Project DefaultTargets="Build" ToolsVersion="4.0" xmlns="http://schemas.microsoft.com/developer/msbuild/2003">
      ]]);
    end,

    writeConfiguration = function(self, config)
        self.writer:Write([[
            <ProjectConfiguration Include="]] .. config.Name .. [[">
              <Configuration>]] .. config.Configuration .. [[</Configuration>
              <Platform>]] .. config.Platform .. [[</Platform>
            </ProjectConfiguration>
        ]]);
    end,

    writeConfigurations = function(self)
        self.writer:Write([[
            <ItemGroup Label="ProjectConfigurations">
            ]]);
        for i, v in ipairs(self.Configurations) do
            self:writeConfiguration(v);
        end
        self.writer:Write([[
            </ItemGroup>
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
        local writeDirFiles = function(elementName, src, regex)
            local filePaths = src:GetPaths(regex)
            for i = 1, #filePaths do
                local fp = filePaths[i];
                if not self:filePathExclude(fp) then
                  if elementName == "None" then
                      self.writer:Write([[<None Include="]] ..
                          fp.AbsolutePath ..
                          [[">
                            <FileType>Document</FileType>
                          </None>]])
                  else
                      self.writer:Write([[<]] .. elementName ..[[ Include="]])
                      self.writer:Write(fp.AbsolutePath .. [[" />]] .. '\n')
                  end
                end
            end
        end

        self.writer:Write([[<ItemGroup>]]);
        for i, v in ipairs(self.sources) do
            writeDirFiles("None", v, [[\.(mcpp)?$]])
        end
        self.writer:Write([[</ItemGroup>]]);

        self.writer:Write([[<ItemGroup>]]);
        for i, v in ipairs(self.sources) do
            writeDirFiles("ClCompile", v, [[\.(c|cc|cpp)?$]])
        end
        writeDirFiles("ClCompile", self.outputPath, [[\.(c|cpp)?$]])
        self.writer:Write([[</ItemGroup>]]);

        self.writer:Write([[<ItemGroup>]]);
        for i, v in ipairs(self.sources) do
            writeDirFiles("ClInclude", v, [[\.(h|hpp)?$]])
        end
        writeDirFiles("ClInclude", self.outputPath, [[\.(h|hpp)?$]])
        self.writer:Write([[</ItemGroup>]]);
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
