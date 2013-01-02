#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <Funcs.h>

BOOST_AUTO_TEST_CASE(WeGotTheFunc)
{
	BOOST_CHECK_EQUAL(5 + 24, Funcs::AddTwoNumbers(5, 24));

}
