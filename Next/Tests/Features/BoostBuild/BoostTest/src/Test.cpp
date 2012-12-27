#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ExampleTests
#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(ExampleeSuite)

static int add(int a, int b) 
{
	return a + b;
}

BOOST_AUTO_TEST_CASE(universeInOrder)
{
    BOOST_CHECK(add(2, 2) == 4);
}

BOOST_AUTO_TEST_SUITE_END()
