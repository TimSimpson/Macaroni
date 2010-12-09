/*#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE LuaUnitTests
#include <boost/test/unit_test.hpp>

#include <Macaroni/Environment/LuaEnvironment.h>
#include <Macaroni/IO/Paths.h>


using Macaroni::Environment::LuaEnvironment;
using Macaroni::IO::Paths;

BOOST_AUTO_TEST_SUITE(LuaUnitTests)

BOOST_AUTO_TEST_CASE(All)
{	
	std::string luaTestsPath("");

	BOOST_CHECK_EQUAL(Paths::GetExePath(), luaTestsPath);
	Macaroni::Environment::LuaEnvironment lua;
	lua.SetPackageDirectory(luaTestsPath);

	std::stringstream ss;
	ss << luaTestsPath << "/Tests.lua";
	lua.ParseFile(ss.str());
	lua.Run();
}


BOOST_AUTO_TEST_SUITE_END()*/

#include <iostream>
#include <boost/test/execution_monitor.hpp>
#include <Macaroni/Environment/LuaEnvironment.h>
#include <boost/test/included/unit_test.hpp>
#include <boost/test/debug.hpp>


using Macaroni::Environment::LuaEnvironment;
using namespace boost::unit_test;



//____________________________________________________________________________//

std::string luaTestsPath;

void run_unit_tests()
{
	Macaroni::Environment::LuaEnvironment lua;
	std::vector<std::string> paths;
	paths.push_back(luaTestsPath);
	paths.push_back(luaTestsPath + "/Generators");
	lua.SetPackageDirectory(paths);
	std::stringstream ss;
	ss << luaTestsPath << "/Tests.lua";
	std::cout << "Beginning Lua Unit Tests at entry file " << luaTestsPath 
		<< "!!" << std::endl;
	lua.ParseFile(ss.str());
	lua.Run();
}


//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int argc, char* argv[] )
{ 
	boost::debug::detect_memory_leaks(false);

	std::cout << "ARG COUNT= " << argc << std::endl;
	for(int i = 0; i < argc; i ++)
	{
		std::cout << i << "=" << argv[i] << std::endl;
	}

	if (argc < 2) 
	{
		throw std::exception("Argument count must be more than 1- where's the path to the Lua tests?!");
	}

	luaTestsPath = std::string(argv[1]);

    test_suite* ts1 = BOOST_TEST_SUITE( "Lua Unit Tests" );
    ts1->add( BOOST_TEST_CASE( &run_unit_tests ) );

    framework::master_test_suite().add( ts1 );

    return 0;
}

