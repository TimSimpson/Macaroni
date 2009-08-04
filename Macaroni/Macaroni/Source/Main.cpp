#include "Macaroni/ME.h"
#include "Macaroni/CmdLine.h"
#include "Macaroni/Compiler.h"
#include "Macaroni/CompilerOptions.h"
#include "Macaroni/Exception.h"
#include "Macaroni/Environment/LuaEnvironment.h"
#include "Gestalt/FileSystem/FileSet.h"
#include <iostream>
#include <tchar.h>

#ifdef COMPILE_TARGET_TESTS
	//#include <boost/test/unit_test.hpp>
#endif 

using namespace Macaroni;
using namespace Macaroni;
using Gestalt::FileSystem::FileSet;

void compileProjectFiles()
{
	FileSet input(boost::filesystem::path("Macaroni"), "\\.mcpp$");
	CompilerOptions options(input, 
							boost::filesystem::path("Macaroni"));
	Compiler::Compile(options);
}

void compileToyFiles()
{
	FileSet input(boost::filesystem::path("RealTest/Input"), "\\.mcpp$");
	CompilerOptions options(input, 
							boost::filesystem::path("RealTest/Output"));
	Compiler::Compile(options);
}

void runLuaTests()
{
	Macaroni::Environment::LuaEnvironment lua;
	lua.ParseFile("Main.lua");
	lua.Run();
}

int _tmain(int argc, _TCHAR* argv[])
{
#ifdef COMPILE_TARGET_TESTS
	//return init_unit_tests();//main(argc, nullptr);
#endif
	
	std::vector<std::string> args;
	args.push_back("Tests/simple.mcpp");
	
	runLuaTests();

	//compileToyFiles();
	
	compileProjectFiles();

	//try
	//{
	//	CmdLine(args);
	//}
	//catch(Macaroni::Exception me)
	//{
	//	std::cerr << "Oh no, an error occured!";
	//	std::cerr << me.
	//}
	std::cout << "Program finished. Press enter to quit.";
	char ch;
	std::cin >> ch;
	return 0;
}

