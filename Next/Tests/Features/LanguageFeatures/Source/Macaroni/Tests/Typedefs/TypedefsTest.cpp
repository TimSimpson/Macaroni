//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Macaroni_Tests_Typedefs
#include <boost/test/unit_test.hpp>
#include <Macaroni/Tests/Typedefs/Doggy.h>
#include <Macaroni/Tests/Typedefs/DoggyList.h>
#include <Macaroni/Tests/Typedefs/DoggyListType.h>

using Macaroni::Tests::Typedefs::Doggy;
using Macaroni::Tests::Typedefs::DoggyList;


BOOST_AUTO_TEST_SUITE(TypedefsSuite)

/**
 * Tests Macaroni's ability to define TypeDefs.
 * More importantly is ability to reference complex types which have, as
 * arguments, other types.
 */
BOOST_AUTO_TEST_CASE(typedefs)
{
	DoggyList dogs;
	dogs.push_back(Doggy("Hallie", 15));
	BOOST_CHECK_EQUAL(dogs.size(), 1);
	BOOST_CHECK_EQUAL(dogs[0].GetDogYears(), 15 * 7);
}




BOOST_AUTO_TEST_SUITE_END()
