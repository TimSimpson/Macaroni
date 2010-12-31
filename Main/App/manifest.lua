id =
{
    group="Macaroni",
    name="Macaroni-App",
    version="0.1.0.6",
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
dependency {group="Macaroni", name="Boost-filesystem", version="1.42.0"}
dependency {group="Macaroni", name="Boost-regex", version="1.42.0"}
    
function bjam()
    --local rtnCode = os.execute("bjam")
    return rtnCode == 0
end
        
function generate()    
    run "Cpp"
    --runGenerator "Generators/LuaGlue"
    run "InterfaceMh"
    run "JamGenerator"
    run "VersionInfoGenerator"
    print "Code Generation successful.  Calling Boost Build."
    if (bjam()) then
        print "~ YOU WIN! ~"
    else
        print "~ YOU LOSE ~"          
    end
end
   
jamArgs = 
{ 	
	ExcludePattern = "Main.cpp *Test.cpp *Tests.cpp .svn",
	ExtraTargets = [[
		#alias tests
		#:
		#[ run ../Source/test/mcpp/Macaroni/Model/NodeTest.cpp library_sources library_dependencies  ]
		#;


	  	exe macaroni 
	  		:	library_sources
	  			library_dependencies
	  			../Source/main/mcpp/Main.cpp
	  			../Source/main/resources/Macaroni.rc
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

function install()	
end