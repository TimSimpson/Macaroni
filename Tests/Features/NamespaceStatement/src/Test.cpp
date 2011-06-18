#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <Macaroni/Tests/NamespaceStatement/ForThoseDaysWhenYouDontWantBraces.h>

using Macaroni::Tests::NamespaceStatement::ForThoseDaysWhenYouDontWantBraces;

BOOST_AUTO_TEST_CASE(MyTestCase)
{		
	ForThoseDaysWhenYouDontWantBraces ftdwydwb;		
	BOOST_CHECK_EQUAL(5, ftdwydwb.five);
}
