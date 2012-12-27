require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".BoostTest",
    version=upper.Version
}

description="Tests using BoostTest."

sources = { "src" }
output = "target"

dependency {group="Macaroni", name="CppStd", version="2003"}
--dependency {group="Macaroni", name="Boost-smart_ptr", version="1.42.0"}
dependency {group="Macaroni", name="Boost.Test", version="1.45.0" }

function generate()
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = ".svn *Test.cpp *Start.cpp",	
	ExtraTargets = [[
		alias tests
			:	[ run ../src/Test.cpp  
					library ]
			;		
		#exe LuaTest 
	  	#	:	library
	  	#		../src/Test.cpp	  			
	  	#	;	
	  	exe App 
	  		:	library
	  			../src/Start.cpp	  			
		#	:	<dependency>tests
	  		;
	  	
	  ]]
};
		
function build()
	run("BoostBuild", jamArgs)
end

function install()
	run("BoostBuild", jamArgs);
	
end


