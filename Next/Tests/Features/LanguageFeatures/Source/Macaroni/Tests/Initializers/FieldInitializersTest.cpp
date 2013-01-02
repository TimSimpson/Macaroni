#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Macaroni_Tests_Initializers_Singleton
#include <boost/test/unit_test.hpp>
#include <Macaroni/Tests/Initializers/Singleton.h>


using Macaroni::Tests::Initializers::Singleton;


BOOST_AUTO_TEST_SUITE(StaticFieldInitializationSuite)

/**
 * Macaroni provides the ability to define fields of classes using the equals
 * sign.  For static fields, this means the definition exists in the cpp file.
 */
BOOST_AUTO_TEST_CASE(member_fields_are_set_within_their_cpp_files)
{
	std::string defaultText(Singleton::GetDefaultText());
	BOOST_REQUIRE_MESSAGE(defaultText == "default text",
						  "Fields must be initialized in their CPP files.");
}

/**
 * "~global" fields are also defined in the CPP file.
 * Private global fields reside in an anonymous namespace.
 */
BOOST_AUTO_TEST_CASE(global_member_fields_are_set_within_their_cpp_files)
{
	const char * text = Singleton::GetDefaultGlobalText();
	std::string actual(text);
	BOOST_REQUIRE_MESSAGE(actual.compare("default global text") == 0,
						  "Global fields must be initialized in their CPP files.");
}

/**
 * Here, a simple implementation of the singleton pattern is created by
 * giving a class a private static member which is initialized to 0.
 * This test asserts that the initializer is place within the cpp file.
 */
BOOST_AUTO_TEST_CASE(a_test_case_for_common_singleton_implementation)
{
	Singleton * s = Singleton::PeekAtHiddenStatic();
	BOOST_REQUIRE_MESSAGE(0 == s,
						  "Singleton instance must be initialized to zero.");
	s = Singleton::GetInstance();
	BOOST_REQUIRE_MESSAGE(s != 0,
						  "After calling GetInstance(), the Singleton should be initialized.");
	BOOST_REQUIRE_MESSAGE(s == Singleton::GetInstance(),
						  "Further calls to GetInstance() return the same value.");
}

BOOST_AUTO_TEST_SUITE_END()
