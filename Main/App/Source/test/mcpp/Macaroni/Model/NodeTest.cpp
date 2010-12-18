#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE NodeTests
#include <boost/test/unit_test.hpp>

#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/Node.h>

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;

int add(int i, int j)
{
    return i + j; 
}

BOOST_AUTO_TEST_SUITE(NodeSuite)

BOOST_AUTO_TEST_CASE(universeInOrder)
{
    BOOST_CHECK(add(2, 2) == 4);
}

BOOST_AUTO_TEST_CASE(makeContext)
{	
	ContextPtr context(new Context("%ROOT%"));
	
	BOOST_CHECK_EQUAL(1, context->GetReferenceCount());	

	
	NodePtr root = context->GetRoot();
	BOOST_CHECK( !!root );
	std::string name = root->GetName();
	BOOST_CHECK_EQUAL(root->GetName(), "%ROOT%");	
}

BOOST_AUTO_TEST_CASE(checkFailure)
{
	BOOST_CHECK_EQUAL(4, add(2,2));
}

BOOST_AUTO_TEST_SUITE_END()
