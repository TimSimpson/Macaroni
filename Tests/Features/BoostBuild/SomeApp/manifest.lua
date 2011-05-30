require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".SomeApp",
    version=upper.Version
}

description="A library."

source "Source"
output = "GeneratedSource"

dependency {group="Macaroni", name="CppStd", version="2003"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group=upper.Group, name=upper.Name .. "SomeLibA", version=upper.Version}
-- dependency {group="Macaroni", name="Tests - BoostBuild - SomeLib B", version="1.0.0.0"}

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = "Main.cpp .svn *Test.cpp Start.cpp",	
	ExtraTargets = [[
		#alias tests
		#	:	[ run ../Source/Test.cpp  
		#			libSources ]
		#	;			
	  	exe App 
	  		:	library_sources
	  			library_dependencies
	  			../Source/Start.cpp	  			
		#	:	<dependency>tests
	  		;
	  	
	  ]],
	  Shared = "true"
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs);
	
end


