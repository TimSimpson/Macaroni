#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>
#include <MagicalPaintBrush.h>

BOOST_AUTO_TEST_CASE(ProveToMeYouKnowTheSecret)
{

	MagicalPaintBrush mPaint;
	BOOST_CHECK_EQUAL(
	    "\n~   ~\n"
		  " --- \n",
	    mPaint.revealSecrets());

}
