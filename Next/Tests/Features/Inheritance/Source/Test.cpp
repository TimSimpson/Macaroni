#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <A.h>
#include <B.h>
#include <VB.h>
#include <C.h>
#include <VC.h>
#include <BC.h>
#include <VBC.h>

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	A a;
	BOOST_CHECK_EQUAL(a.MethodA(), 'a');
	BOOST_CHECK_EQUAL(a.MethodVA(), "va");

	B b;
	BOOST_CHECK_EQUAL(b.MethodB(), 'b');
	BOOST_CHECK_EQUAL(b.MethodA(), 'a');
	BOOST_CHECK_EQUAL(b.MethodVA(), "va");
	BOOST_CHECK_EQUAL(b.MethodVB(), "vb");

	C c;
	BOOST_CHECK_EQUAL(c.MethodC(), 'c');
	BOOST_CHECK_EQUAL(c.MethodA(), 'a');

	VB vb;
	BOOST_CHECK_EQUAL(vb.MethodVB(), 'x');
	BOOST_CHECK_EQUAL(vb.MethodVA(), "override-a");

	BC bc;
	//BOOST_CHECK_EQUAL(bc.MethodA(), 'a'); -- is ambigious
	BOOST_CHECK_EQUAL(bc.B::MethodA(), 'a');
	BOOST_CHECK_EQUAL(bc.C::MethodA(), 'a');
	BOOST_CHECK_EQUAL(bc.MethodB(), 'b');
	BOOST_CHECK_EQUAL(bc.MethodC(), 'c');

	VBC vbc;
	BOOST_CHECK_EQUAL(vbc.VB::MethodA(), 'a');
	BOOST_CHECK_EQUAL(vbc.VC::MethodA(), 'a');
	BOOST_CHECK_EQUAL(vbc.MethodA(), 'a');
	BOOST_CHECK_EQUAL(vbc.MethodVB(), 'x');
	BOOST_CHECK_EQUAL(vbc.MethodVC(), 'y');

}
