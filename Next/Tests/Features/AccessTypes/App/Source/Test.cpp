
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <Something.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	Something s = Something::Create(5, 7);
	BOOST_CHECK_EQUAL(s.GetX(), 5);
	BOOST_CHECK_EQUAL(s.GetY(), 7);
	// If a class is exported, all of its members must be exported too.
	// This will be more difficult than I originally imagined.
}
