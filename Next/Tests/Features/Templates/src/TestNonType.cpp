#include <exception>
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

/** Non-Type Template Parameters
 * number or enum
 * pointer to function or variable
 * reference to function or variable
 * lvalue reference to function or variable
 * pointer to member
 * nullptr
 */

template<int N>
class CharBuffer
{
public:
	char data[N];

	CharBuffer()
	{
		clear();
	}

	~CharBuffer()
	{
		clear();
	}

	void clear()
	{
		for (int i = 0; i < N; i ++)
		{
			data[i] = '\0';
		}
	}

	char & operator[](const unsigned int & index)
	{
		if (index < 0 || index >= N)
		{
			throw std::exception("Out of bounds!");
		}
		return data[index];
	}

};

BOOST_AUTO_TEST_CASE(template_nontype_number)
{
	CharBuffer<100> buf;
	for (int i = 0; i < 100; i ++)
	{
		BOOST_CHECK_EQUAL('\0', buf[i]);
	}

	buf[0] = 'h';
	buf[1] = 'i';

	BOOST_CHECK_EQUAL('h', buf[0]);
	BOOST_CHECK_EQUAL('i', buf[1]);

	for (int i = 2; i < 100; i ++)
	{
		BOOST_CHECK_EQUAL('\0', buf[i]);
	}
}


template<std::string & str>
class StupidGlobalUser
{
public:
	StupidGlobalUser()
	{
		str = "HELLO!";
	}

	~StupidGlobalUser()
	{
		str = "GOOD BYE";
	}

	void Modify(const char * msg)
	{
		str = msg;
	}
};

std::string msg = "START";

BOOST_AUTO_TEST_CASE(template_nontype_pointer_to_var)
{
	// Hmm, a local variable can't be used as the type argument.
	// Makes sense I guess! No way to check if the templated class instance
	// would be passed around past the object scope.

	BOOST_CHECK_EQUAL(msg, "START");
	{
		StupidGlobalUser<msg> dumb;
		BOOST_CHECK_EQUAL(msg, "HELLO!");
		dumb.Modify("Yo");
		BOOST_CHECK_EQUAL(msg, "Yo");
	}
	BOOST_CHECK_EQUAL(msg, "GOOD BYE");


}
