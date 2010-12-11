//// This is a test to make sure Boost Test is working.
//
//// Some quick notes on how I made this work:
//// First off, the following directives must be known to the compiler:
////		BOOST_TEST_MAIN
////		BOOST_TEST_DYN_LINK
//// BOOST TEST provides a main that is not run unless you use the dynamic option.
////
//// Second, the DLL for boost has to be copied.  I added a pre-build event that
//// looks like this:
//// copy "F:\Tools\boost_1_37_0\stage\lib\boost_unit_test_framework-vc90-mt-gd-1_37.dll" "$(TargetDir)"
////
//// Third, a post-build event will call the tests and put them in the VS error pane:
//// "$(TargetDir)\$(TargetName).exe" -result_code=no -report_level=no
//
////#define BOOST_TEST_MAIN
////#define BOOST_TEST_MAIN
//#define BOOST_TEST_MODULE MacaroniTests
//#include <boost/test/unit_test.hpp>
//
//int add( int i, int j ) { return i+j; }
//
//BOOST_AUTO_TEST_CASE( my_test )
//{
//    // seven ways to detect and report the same error:
//    BOOST_CHECK( add( 2,2 ) == 4 );        // #1 continues on error
//
//    BOOST_REQUIRE( add( 2,2 ) == 4 );      // #2 throws on error
//
//    if( add( 2,2 ) != 4 )
//      BOOST_ERROR( "Ouch..." );            // #3 continues on error
//
//    if( add( 2,2 ) != 4 )
//      BOOST_FAIL( "Ouch..." );             // #4 throws on error
//
//    if( add( 2,2 ) != 4 ) throw "Ouch..."; // #5 throws on error
//
//    BOOST_CHECK_MESSAGE( add( 2,2 ) == 4,  // #6 continues on error
//                         "add(..) result: " << add( 2,2 ) );
//
//    BOOST_CHECK_EQUAL( add( 2,2 ), 4 );	  // #7 continues on error
//}