#include <vector>

#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

/** Types of Templates:
 *  type
 *  non-type
 *  template - These are templates themselves.
 */

template<typename E, template <typename name_not_used> class Container>
class SuperContainer
{
public:

	Container<E> c;
};

//http://codingcastles.blogspot.com/2009/04/things-i-didnt-know-about-c-template.html
//http://stackoverflow.com/questions/213761/what-are-some-uses-of-template-template-parameters-in-c/214900#214900
BOOST_AUTO_TEST_CASE(template_template)
{
	//SuperContainer<intgitg, exitstd::vector> UseVec;
	//UseVec.c.push_back(50);
}
