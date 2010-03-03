#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE ContextTests
#include <boost/test/unit_test.hpp>

#include "Context.h"
#include "Node.h"

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;

BOOST_AUTO_TEST_SUITE(ContextSuite)

BOOST_AUTO_TEST_CASE(CreateRoot_Test)
{	
	ContextPtr context(new Context("%ROOT%"));
	
	BOOST_CHECK_EQUAL(1, context->GetReferenceCount());	

	NodePtr root = context->GetRoot();
	BOOST_CHECK( !!root );
	std::string name = root->GetName();
	BOOST_CHECK_EQUAL(root->GetName(), "%ROOT%");	
}

BOOST_AUTO_TEST_CASE(ParseComplexName_will_morph_unknown_Nodes_into_Namespaces)
{
	ContextPtr context(new Context("{ROOT}"));
    NodePtr d = context->GetRoot()->FindOrCreate("A::B::C::D");
    BOOST_CHECK_EQUAL(d->GetFullName(), "A::B::C::D");       
    NodePtr d_c = d->GetNode();
	BOOST_CHECK_EQUAL(d_c->GetFullName(), "A::B::C");
    NodePtr d_b = d_c->GetNode();
    BOOST_CHECK_EQUAL(d_b->GetFullName(), "A::B");
    local d_a = d_b.Node;
    Test.assert("A", d_a.FullName);       
    Test.assert(1, #(context.Root.Children));
   
    -- Must reuse A, and morph it into an Namespace...
    -- I.. am.. morphing...
    local a = context.Root:FindOrCreate("A");
    Test.assert("A", a.FullName);
    Test.assert(a, d_a);
    Test.assert(1, #(context.Root.Children));
}

BOOST_AUTO_TEST_SUITE_END()
