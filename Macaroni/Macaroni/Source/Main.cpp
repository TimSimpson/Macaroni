#include "Macaroni/ME.h"
#include "Macaroni/CmdLine.h"
#include "Macaroni/Compiler.h"
#include "Macaroni/CompilerOptions.h"
#include "Macaroni/Exception.h"
#include "Macaroni/Environment/LuaEnvironment.h"
#include "Gestalt/FileSystem/FileSet.h"
#include <iostream>
#include <sstream>
#include <tchar.h>

#ifdef COMPILE_TARGET_TESTS
	//#include <boost/test/unit_test.hpp>
#endif 

using namespace Macaroni;
using namespace Macaroni;
using Gestalt::FileSystem::FileSet;

void compileProjectFiles()
{
	FileSet input(boost::filesystem::path("../Source"), "\\.mcpp$");
	CompilerOptions options(input, 
							boost::filesystem::path("..\\GeneratedSource"));
	Compiler::Compile(options);
}

void compileToyFiles()
{
	FileSet input(boost::filesystem::path("../RealTest/Input"), "\\.mcpp$");
	CompilerOptions options(input, 
							boost::filesystem::path("../RealTest/Output"));
	Compiler::Compile(options);
}

void execute(const std::string & inputPath, const std::string & outputPath)
{
	FileSet input(boost::filesystem::path(inputPath), "\\.mcpp$");
	CompilerOptions options(input, 
							boost::filesystem::path(outputPath));
	Compiler::Compile(options);
}

void runLuaTests()
{
	Macaroni::Environment::LuaEnvironment lua;
	lua.ParseFile("Main.lua");
	lua.Run();
}

void convert(std::wstring & original, std::string & rtnString)
{
	std::stringstream ss;	
	for(unsigned int i = 0; i < original.size(); i ++)
	{
		ss << ((char)original[i]);
	}
	rtnString = ss.str();
}

int _tmain(int argc, const _TCHAR * argv[])//_TCHAR* argv[])
{
#ifdef COMPILE_TARGET_TESTS
	//return init_unit_tests();//main(argc, nullptr);
#endif

	bool debugMode = false;

	if (argc > 0)
	{
		std::string inputPath;
		std::string outputPath;
		for (int i = 0; i < argc; i ++)
		{			
			const TCHAR * tcharString = argv[i];
			std::wstring arg(tcharString);
			if (arg == _TEXT("debug"))
			{
				debugMode = true;
				runLuaTests();
			}
			else if (arg == _TEXT("-inputDirectory"))
			{
				if (i == argc - 1)
				{
					std::cerr << "No input specified.";
				}
				else
				{
					convert(std::wstring(argv[i + 1]), inputPath);
				}
			}
			else if (arg == _TEXT("-outputDirectory"))
			{
				if (i == argc - 1)
				{
					std::cerr << "No output specified.";
				}
				else
				{
					convert(std::wstring(argv[i + 1]), outputPath);
				}
			}
		}		
		if (outputPath.empty())
		{
			std::cerr << "Missing option -outputDirectory.";
		}
		else if (inputPath.empty())
		{
			std::cerr << "Missing option -inputDirectory.";
		}
		else
		{
			execute(inputPath, outputPath);
		}
		return 0;
	}

	
	//args.push_back("../Tests/simple.mcpp");	

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
	std::cout << "Program finished.";
	if (debugMode)
	{
		std::cout << "Press enter to quit.";
		char ch;
		std::cin >> ch;
	}
	return 0;
}

