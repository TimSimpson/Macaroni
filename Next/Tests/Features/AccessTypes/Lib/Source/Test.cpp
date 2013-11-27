//#include <Config_AccessTypesLib.h>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <Something.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	Something s = Something::Create(5, 7);
	BOOST_CHECK_EQUAL(s.GetX(), 5);
	BOOST_CHECK_EQUAL(s.GetY(), 7);
}
