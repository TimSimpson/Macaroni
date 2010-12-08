#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TurtleTest
#include <boost/test/unit_test.hpp>
#include <boost/filesystem/convenience.hpp> 
#include "Turtle.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <sstream>

using boost::filesystem::create_directories;
using boost::filesystem::path;
using boost::shared_ptr;
using std::string;
using std::stringstream;
using Macaroni::Reptile::Turtle;

BOOST_AUTO_TEST_SUITE(TurtleSuite)

BOOST_AUTO_TEST_CASE(make_sure_turtle_works)
{	
	path p("./");
	Turtle t(p);
	BOOST_CHECK_MESSAGE(t.getNumber() == 50, "Must be 50.");
}

BOOST_AUTO_TEST_SUITE_END()
