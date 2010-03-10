#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Macaroni_Tests_Blocks
#include <boost/test/unit_test.hpp>
#include <Macaroni/Tests/Blocks/Fsm.h>


using Macaroni::Tests::Blocks::Fsm;


BOOST_AUTO_TEST_SUITE(BlocksSuite)

/** 
 * Macaroni Blocks allow you to insert a chunk of unfiltered C++ code into 
 * the generate H or CPP file.
 * This is intended as a tactic of last resort for cases where you'd need to
 * do a few things Macaroni can't handle but would still like to use it.
 */
BOOST_AUTO_TEST_CASE(h_file_blocks_work)
{	
	Fsm fsm;
	fsm.Execute();
	BOOST_CHECK_MESSAGE(GETSTATE(fsm) == ODD, 
		"The FSM should have set its state to 1 following the first call to Execute.");
	fsm.Execute();
	BOOST_CHECK_MESSAGE(GETSTATE(fsm) == EVEN, 
		"The FSM should have set its state back to 0 following the second call to Execute.");	
}

BOOST_AUTO_TEST_SUITE_END()
