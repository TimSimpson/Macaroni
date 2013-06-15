#define BOOST_TEST_MODULE Macaroni_Tests_Consts
#include <boost/test/unit_test.hpp>
#include <Macaroni/Tests/Const/Cat.h>
#include <Macaroni/Tests/Const/Dog.h>

using Macaroni::Tests::Const::Cat;
using Macaroni::Tests::Const::Dog;


BOOST_AUTO_TEST_SUITE(ConstSuite)

/**
 * This is a test for the "const" type modifier.
 * The class Cat has a method "GetFeet" which must be declared const; if not,
 * our const reference will not be able to access it.
 * Additionally we create an instance of class "Dog" which has a const reference
 * to a Cat, and access Cat vicariously through Dog.
 */
BOOST_AUTO_TEST_CASE(h_file_blocks_work)
{
	Cat cat;
	const Cat & constRef = cat;
	constRef.GetFeet();
	Dog dog(cat);
	dog.GetCatFeet();
	BOOST_CHECK_MESSAGE(dog.GetCatFeet() == 4,
		"Must be able to access with const reference.");

}

BOOST_AUTO_TEST_SUITE_END()
