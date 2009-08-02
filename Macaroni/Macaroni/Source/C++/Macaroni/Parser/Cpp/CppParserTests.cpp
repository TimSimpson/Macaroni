#ifdef COMPILE_TARGET_TESTS

//#define BOOST_TEST_MODULE Macaroni_Parser_Cpp_CppParser
//#define BOOST_TEST_NO_MAIN
//#define BOOST_TEST_MODULE Pee
#include <boost/test/auto_unit_test.hpp>


#include "CppParser.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

BOOST_AUTO_TEST_CASE( PeePee )
{
	
    BOOST_CHECK( 1 == 4 );        // #1 continues on error

	BOOST_CHECK( 2 == 4 );        // #1 continues on error
}

BOOST_AUTO_TEST_CASE( Gas )
{
	 
    BOOST_CHECK( 0 == 4 );        // #1 continues on error

	BOOST_CHECK( 2 == 4 );        // #1 continues on error
} 

END_NAMESPACE

#endif