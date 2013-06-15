#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	float x = 1.4f;
	BOOST_CHECK_CLOSE(x, 1.4f, 0.0001f);
	BOOST_CHECK(x != 0.0f);
	BOOST_CHECK_EQUAL((int)x, 1);
}
