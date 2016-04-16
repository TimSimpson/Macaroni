#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <Util/Func.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	BOOST_CHECK_EQUAL(::Add(2,2), 4);

	BOOST_CHECK_EQUAL(::Some::Weird::Place::Dumb(), 256);

	BOOST_CHECK_EQUAL(Util::Func::AddStuff(57), 256 + 57);
}
