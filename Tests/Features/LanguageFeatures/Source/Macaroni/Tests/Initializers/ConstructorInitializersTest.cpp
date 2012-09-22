//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Macaroni_Tests_Initializers_ConstructorInitializers
#include <boost/test/unit_test.hpp>
#include <Macaroni/Tests/Initializers/Poco.h>


using Macaroni::Tests::Initializers::Poco;


BOOST_AUTO_TEST_SUITE(ConstructorsInitializersSuite)

BOOST_AUTO_TEST_CASE(fields_set_in_constructors)
{
	Poco p("Patches", 25);
	const Poco * pp = &p;
	BOOST_CHECK_EQUAL(25, pp->GetAge());
	BOOST_CHECK_EQUAL(pp->GetName(), "Patches");
}

BOOST_AUTO_TEST_SUITE_END()
