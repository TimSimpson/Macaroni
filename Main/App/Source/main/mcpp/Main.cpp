#include "Macaroni/ME.h"
#include "Macaroni/Build/CmdLine.h"
#include "Macaroni/Build/MCompiler.h"
#include "Macaroni/Build/MCompilerOptions.h"
#include "Macaroni/Environment/Console.h"
#include "Macaroni/Exception.h"
#include "Macaroni/Environment/LuaEnvironment.h"
#include "Macaroni/Environment/Messages.h"
#include "Macaroni/Platform/Windows/Strings.h"
#include "Macaroni/IO/FileSet.h"
#include <iostream>
#include <sstream>
#include <tchar.h>
#include <Macaroni/VersionNo.h>

#ifdef COMPILE_TARGET_TESTS
	//#include <boost/test/unit_test.hpp>
#endif

using namespace Macaroni;
using namespace Macaroni;
using Macaroni::Build::CmdLine;
using Macaroni::Environment::Console;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Macaroni::IO::FileSet;
using Macaroni::Environment::Messages;
using Macaroni::Platform::Windows::NonWindowsString;
using Macaroni::Platform::Windows::WindowsString;

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
	std::cout << MACARONI_FILE_DESCRIPTION << std::endl;
	std::cout << MACARONI_COPYRIGHT << std::endl;
	std::cout << "Version " << MACARONI_VERSION_STRING << 
		" built at " << BUILD_TIMESTAMP_LOCAL << std::endl;
	std::cout << std::endl;

	std::vector<const std::string> convertedArgs;
	for (int i = 0; i < argc; i ++)
	{
		NonWindowsString convertedString(argv[i]);
		convertedArgs.push_back(convertedString.get());
	}

	Console output;

	CmdLine cmd(convertedArgs, output);

	if (cmd.StartPrompt())
	{
		std::cout << 
			"\"startPrompt\" was specified- waiting for user to enter text and "
			"hit enter." << std::endl;
		std::string stuff;
		std::cin >> stuff;
	}

	int returnCode = 1;
	if (cmd.StartPrompt()) // Not running in a try / catch makes it easier to debug
	{
		if (cmd.Execute())
		{		
			returnCode = 0; // success		
		}
	}
	else
	{
		try
		{
			if (cmd.Execute())
			{		
				returnCode = 0; // success		
			}
		} catch(std::exception & ex)
		{
			std::cerr << "An error occured.\n" << std::endl;
			std::cerr << ex.what()  << std::endl;
		}
	}

	std::cout << std::endl << "Program finished." << std::endl;
	if (cmd.EndPrompt())
	{
		std::cout << "press enter to quit.";
		char ch;
		std::cin >> ch;
	}
	
	return returnCode;
}

