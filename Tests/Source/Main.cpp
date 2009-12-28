#include <Fruits/Apple.h>
#include <iostream>
#include <sstream>
#include <tchar.h>

using Fruits::Apple;

int _tmain(int argc, const _TCHAR * argv[])
{
	std::cout << "HELLO WORLD!" << std::endl;
	Apple apple;
	std::cout << apple.FindWormCount("BLAH!");
	std::cout << "The End" << std::endl;
}