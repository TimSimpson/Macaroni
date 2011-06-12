require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".Lib",
    version=upper.Version
}

description=[[
Macaroni generates implementation files which have their own distinct version
of the header.  This is so they can include private function symbols (and maybe 
other symbols if this idea turns out not be terrible) and make the publicly 
exposed headers simpler.
]]

dependency {group="Macaroni", name="Boost-filesystem", version="1.46.1"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.46.1"}
dependency {group="Macaroni", name="CppStd", version="2003"}
-- dependency {group = "Lua", name = "Lua", version = "5.1.4" }

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"    
end

jamArgs = 
{ 	
	ExcludePattern = "Main.cpp .svn *Test*.cpp",
	ExtraTargets = [[		
		#lib boost_unit_test_framework : : <name>boost_unit_test_framework ;
		
	    #alias test_dependencies
	#    	: "]] .. properties.boost.current["path"] 
	    	      .. [[/libs/test/build//boost_unit_test_framework"  
	  #      :	    	
	  #      ;
	    	
		#unit-test test
		#    : library
		#      test_dependencies		      
		#      ../Source/Test.cpp		    
		#    ;
		    
		# THIS STUPID THING WORKED ONCE YESTERDAY AND THEN NEVER AGAIN... URG...
		# This should fail because it access the ~hidden function.
		#compile-fail 
		#	library
		#	test_dependencies 
		#	../Source/Test2.cpp 
		#;	  	
	  ]],
	  Shared= True,
	  Tests = { "Test.cpp"}, 
	};
		
function build()
	run("BoostBuild", jamArgs)
end

function test()	
	print("HI")
    -- args = { ExcludePattern=jamArgs.ExcludePattern,
	--         ExtraTargets=jamArgs.ExtraTargets,
	--         --CmdLine="test" 
	--       } 
	run("BoostBuild", jamArgs); -- args);
end

function install()
end


