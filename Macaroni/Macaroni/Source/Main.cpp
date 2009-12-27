#include "Macaroni/ME.h"
#include "Macaroni/Build/CmdLine.h"
#include "Macaroni/Build/MCompiler.h"
#include "Macaroni/Build/MCompilerOptions.h"
#include "Macaroni/Environment/Console.h"
#include "Macaroni/Exception.h"
#include "Macaroni/Environment/LuaEnvironment.h"
#include "Macaroni/Environment/Messages.h"
#include "Gestalt/FileSystem/FileSet.h"
#include <iostream>
#include <sstream>
#include <tchar.h>

#ifdef COMPILE_TARGET_TESTS
	//#include <boost/test/unit_test.hpp>
#endif 

using namespace Macaroni;
using namespace Macaroni;
using Macaroni::Build::CmdLine;
using Macaroni::Environment::Console;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Gestalt::FileSystem::FileSet;
using Macaroni::Environment::Messages;
//
//void compileProjectFiles()
//{
//	FileSet input(boost::filesystem::path("../Source"), "\\.mcpp$");
//	MCompilerOptions options(input, 
//							boost::filesystem::path("..\\GeneratedSource"));
//	MCompiler compiler;
//	compiler.Compile(options);
//}
//
//void compileToyFiles()
//{
//	FileSet input(boost::filesystem::path("../RealTest/Input"), "\\.mcpp$");
//	MCompilerOptions options(input, 
//							boost::filesystem::path("../RealTest/Output"));
//	MCompiler compiler;
//	compiler.Compile(options);
//}
//
//void execute(const std::string & inputPath, const std::string & outputPath)
//{
//	FileSet input(boost::filesystem::path(inputPath), "\\.mcpp$");
//	MCompilerOptions options(input, 
//							boost::filesystem::path(outputPath));
//	MCompiler compiler;
//	compiler.Compile(options);
//}
//
////obsolete
//void runLuaTests() 
//{
//	Macaroni::Environment::LuaEnvironment lua;
//	lua.ParseFile("Main.lua"); 
//	lua.Run();
//}

void convert(std::wstring & original, std::string & rtnString)
{
	std::stringstream ss;	  
	for(unsigned int i = 0; i < original.size(); i ++)
	{
		ss << ((char)original[i]);
	}
	rtnString = ss.str();
}

/** Wondering why this isn't in a seperate class, future ME?
 * Its because of the non-standard way Windows handles strings.
 * Not sure what to do with this code, but its def. not platform-independent. */
int _tmain(int argc, const _TCHAR * argv[])//_TCHAR* argv[])
{
#ifdef COMPILE_TARGET_TESTS
	//return init_unit_tests();//main(argc, nullptr);
#endif
	
	std::cout << Messages::Get("Macaroni.Intro");

	std::vector<const std::string> convertedArgs;
	for (int i = 0; i < argc; i ++)
	{
		std::string convertedString("");
		convert(std::wstring(argv[i]), convertedString);
		convertedArgs.push_back(convertedString);
	}
	
	Console output;
	CmdLine cmd(convertedArgs, output);
	try 
	{
		cmd.Execute();
	} catch(Macaroni::Exception ex) 
	{
		std::cerr << "MACARONI EXCEPTION!" << std::endl;
		std::cerr << ex.GetMessage() << std::endl;
	}catch(Macaroni::Exception * ex) 
	{
		std::cerr << "MACARONI EXCEPTION!" << std::endl;
		std::cerr << ex->GetMessage() << std::endl;
	}

	std::cout << std::endl << "Program finished." << std::endl;
	if (cmd.EndPrompt())
	{	
		std::cout << "press enter to quit.";
		char ch;
		std::cin >> ch;
	}
	//bool debugMode = false;
	//bool endPrompt = false;

	//if (argc > 0)
	//{
	//	std::string inputPath;
	//	std::string outputPath;
	//	for (int i = 0; i < argc; i ++)
	//	{			 
	//		const TCHAR * tcharString = argv[i];
	//		std::wstring arg(tcharString);
	//		if (arg == _TEXT("debug"))
	//		{
	//			debugMode = true;
	//			runLuaTests();   
	//		}
	//		else if (arg == _TEXT("-luaTests"))
	//		{
	//			if (i == argc - 1) 
	//			{
	//				std::cerr << "No input specified.";
	//			}
	//			else 
	//			{ 
	//				std::string testPath;
	//				convert(std::wstring(argv[i + 1]), testPath);
	//				runLuaTests();
	//			}
	//		}
	//		else if (arg == _TEXT("-inputDirectory"))
	//		{
	//			if (i == argc - 1)
	//			{
	//				std::cerr << "No input specified.";
	//			}
	//			else
	//			{
	//				convert(std::wstring(argv[i + 1]), inputPath);
	//			}
	//		}
	//		else if (arg == _TEXT("-outputDirectory"))
	//		{
	//			if (i == argc - 1)
	//			{
	//				std::cerr << "No output specified.";
	//			}
	//			else
	//			{
	//				convert(std::wstring(argv[i + 1]), outputPath);
	//			}
	//		}
	//		else if (arg == _TEXT("-endprompt"))
	//		{
	//			endPrompt = true;
	//		}
	//	}		
	//	if (outputPath.empty())
	//	{
	//		std::cerr << "Missing option -outputDirectory.";
	//	}
	//	else if (inputPath.empty())
	//	{
	//		std::cerr << "Missing option -inputDirectory.";
	//	}
	//	else
	//	{
	//		try
	//		{
	//			execute(inputPath, outputPath);
	//		}
	//		catch(Macaroni::Exception ex)
	//		{
	//			std::cerr << "UNHANDLED EXCEPTION:\n";
	//			std::cerr << "Msg:" << ex.GetMessage() << "\n";
	//			std::cerr << "Src:" << ex.GetSource() << "\n";
	//		}
	//		catch(Macaroni::Exception * ex)
	//		{
	//			std::cerr << "UNHANDLED EXCEPTION:\n";
	//			std::cerr << "Msg:" << ex->GetMessage() << "\n";
	//			std::cerr << "Src:" << ex->GetSource() << "\n";
	//		}
	//	}		
	//}
	//else
	//{

	//	
	//	//args.push_back("../Tests/simple.mcpp");	

	//	//compileToyFiles();
	//	
	//	compileProjectFiles();
	//
	//	//try
	//	//{
	//	//	CmdLine(args);
	//	//}
	//	//catch(Macaroni::Exception me)
	//	//{
	//	//	std::cerr << "Oh no, an error occured!";
	//	//	std::cerr << me.
	//	//}
	//}

	////std::cout << "Program finished.";
	//if (endPrompt)
	//{
	//	std::cout << "press enter to quit.";
	//	char ch;
	//	std::cin >> ch;
	//}
	//return 0;
}

