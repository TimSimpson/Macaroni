#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <something.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	Something s = Something::Create(5, 7);
	s.SetX(5);
}
