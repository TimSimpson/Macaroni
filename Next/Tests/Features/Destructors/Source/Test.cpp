#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <Destroyer.h>
#include <InlineDestroyer.h>
#include <VDestroyer.h>
#include <string>
#include <SubclassVDestroyer.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	std::string m1 = "m1";
	BOOST_CHECK_EQUAL(m1, "m1");

	{
		Destroyer instance(m1);
		BOOST_CHECK_EQUAL(m1, "HELLO");
	}
	BOOST_CHECK_EQUAL(m1, "GOOD-BYE");

	{
		VDestroyer instance(m1);
		BOOST_CHECK_EQUAL(m1, "V-HELLO");
	}
	BOOST_CHECK_EQUAL(m1, "V-HELLO_V!");

	{
		SubclassVDestroyer instance(m1);
		BOOST_CHECK_EQUAL(m1, "V-HELLO");
	}
	BOOST_CHECK_EQUAL(m1, "ADIOS, _V!");

	{
		InlineDestroyer instance(m1);
		BOOST_CHECK_EQUAL(m1, "HELLO from inline");
	}
	BOOST_CHECK_EQUAL(m1, "inline works");


}
