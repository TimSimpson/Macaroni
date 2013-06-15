#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

/** Types of Templates:
 *  type
 *  non-type
 *  template - These are templates themselves.
 */

template<class T>
class HoldT
{
public:
	HoldT(T t)
		: t(t)
	{}

	T & GetT()
	{
		return t;
	}

private:
	T t;
};


BOOST_AUTO_TEST_CASE(template_type)
{
	typedef HoldT<char> HoldChar;
	HoldChar a('c');
	BOOST_CHECK_EQUAL('c', a.GetT());
}
