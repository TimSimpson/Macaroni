#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SomeAppTest
#include <boost/test/unit_test.hpp>

#include <SomeLibA/Foo.h>
#include <SomeLibB/Bar.h>

using SomeLibA::Foo;
using SomeLibB::Bar;

BOOST_AUTO_TEST_SUITE(SomeAppTest)

BOOST_AUTO_TEST_CASE(SomeApp)
{	
	Bar bar(25);
	Foo foo(bar);
	BOOST_CHECK_EQUAL(25, foo.GetNumber()) ;
}

BOOST_AUTO_TEST_SUITE_END()
