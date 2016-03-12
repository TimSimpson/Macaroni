
#include "Amigo.h"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(ConstructorOverload0)
{
	Amigo one;
	BOOST_CHECK_EQUAL(one.GetX(), 1);
	BOOST_CHECK_EQUAL(one.GetY(), 2);
}


BOOST_AUTO_TEST_CASE(ConstructorOverload1)
{
	Amigo two(255, 255);
	BOOST_CHECK_EQUAL(two.GetX(), 255);
	BOOST_CHECK_EQUAL(two.GetY(), 255);

	Amigo three = two;
	BOOST_CHECK_EQUAL(three.GetX(), 255);
	BOOST_CHECK_EQUAL(three.GetY(), 255);
}
