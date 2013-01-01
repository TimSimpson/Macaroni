#include <Fruits/Apple.h>
#include <Macaroni/Tests/Typedefs/Doggy.h>
#include <iostream>
#include <sstream>

#ifdef BOOST_WINDOWS

#include <tchar.h>

#endif

using Fruits::Apple;
using Macaroni::Tests::Typedefs::DoggyList;

// There is no real reason to use the Windows style argument list here,
// but Boost config is easy enough to check it sticks around as a curiosity.

#ifdef BOOST_WINDOWS

int _tmain(int argc, const _TCHAR * argv[])

#else

int main(int argc, const char * argv[])

#endif
{

	std::cout << "HELLO WORLD!" << std::endl;
	Apple apple;
	std::cout << apple.FindWormCount("BLAH!");


	std::cout << "The End" << std::endl;
}
