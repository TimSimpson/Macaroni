require "Macaroni.IO.Path"
Path = Macaroni.IO.Path;

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name="Macaroni.App",
    version=upper.Version,
    author="Tim Simpson"
}

description= [[  
    Macaroni is a builder / parser for C++ that aims to increase
    DRYness by slimming down boilerplate and eliminating the need to store
    redundant information.
]]

-- Bug- the Boost Build builder only looks at the last source path.
sources = { "Source/main/lua", "Source/main/resources", "Source/main/mcpp" }
output = "GeneratedSource"
--releasePath = "../" .. properties.macaroni.releasepath .. "/App";

-- Don't trust Macaroni's own dependencies for these, not yet at least.
--dependency { group="Macaroni", name="Boost", version="1.39.0-0" }
--dependency { group="Macaroni", name="CppStd", version="1.0.0.0" }
dependency {group="Macaroni", name="Boost-filesystem", version="1.46.1"}
dependency {group="Macaroni", name="Boost-regex", version="1.46.1"}
        
function generate()    
    run "Cpp"
    --runGenerator "Generators/LuaGlue"
    run "InterfaceMh"
    run "JamGenerator"
    run "VersionInfoGenerator"       
end
   
jamArgs = 
{ 	
	ExcludePattern = "Main.cpp *Test.cpp *Tests.cpp .svn",
	ExtraTargets = [[
		#alias tests
		#:
		#[ run ../Source/test/mcpp/Macaroni/Model/NodeTest.cpp library_sources library_dependencies  ]
		#;

		install installmsgs
			:	../Source/main/resources/Messages.txt
			:	<variant>debug:<location>release/debug
				<variant>release:<location>release/release
			;

	  	exe macaroni 
	  		:	library_sources
	  			library_dependencies
	  			../Source/main/mcpp/Main.cpp
	  			../Source/main/resources/Macaroni.rc
            :
				
	  		;
	  		
	  	install final 
        :	macaroni		       
        :	<install-dependencies>on <install-type>EXE
          #<install-type>LIB
          <variant>debug:<location>release/debug
          <variant>release:<location>release/release          		
        ;
	  ]]
	};
		
function build()	
	run("BoostBuild", jamArgs)
end

function test()
	-- F:\Work\Lp3\Projects\Macaroni\Code\trunk\Main\App>GeneratedSource\release\debug\macaroni.exe luaTests Source\test\lua generatorPath ..\Generators
	local currentPath = Path.New(manifestDirectory);
	local exePath = currentPath:NewPathForceSlash(
		output .. "/release/debug/macaroni.exe");
	local luaTestsPath = currentPath:NewPathForceSlash(
		"Source/test/lua");
	local cmd = exePath.AbsolutePath .. " luaTests " .. luaTestsPath.AbsolutePath;
	print("Running Lua tests:");
	print(cmd);
	os.execute(cmd);
end

function install()	
end
