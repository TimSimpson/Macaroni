require "os"
require "Macaroni.Model.Library"

upper = getUpperLibrary();
id =
{
    group=upper.Group,
    name=upper.Name .. ".LanguageFeatures.OperatorOverloading",
    version=upper.Version
}

description="Proof of concept which overloads the operators of a class."

dependency {group="Macaroni", name="Boost-filesystem", version="1.52"}
dependency {group="Macaroni", name="Boost-smart_ptr", version="1.52"}
dependency {group="Macaroni", name="CppStd", version="2003"}
-- dependency {group = "Lua", name = "Lua", version = "5.1.4" }

function generate()
    run("HtmlView");
    run "InterfaceMh"
    run "Cpp"
end

jamArgs =
{
	ExcludePattern = "Main.cpp .svn *Test.cpp",
	ExtraTargets = [[
		#lib boost_unit_test_framework : : <name>boost_unit_test_framework ;

	    alias test_dependencies
	    	: #"F:/Tools/boost_1_46_1/stage/lib"
	    	  #"F:/Tools/boost_1_46_1/libs/test/build//boost_unit_test_framework"
	    	  "]] .. properties.boost.current["path"]
	    	      .. [[/libs/test/build//boost_unit_test_framework"
	    	  #boost_unit_test_framework
	    	;

		unit-test test
		    : test_dependencies
		      ../Source/Test.cpp
		    ;
	  ]],
	  Shared= True
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


