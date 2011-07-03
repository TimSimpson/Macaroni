/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "Macaroni/ME.h"
#include "Macaroni/Build/CmdLine.h"
#include "Macaroni/Build/MCompiler.h"
#include "Macaroni/Build/MCompilerOptions.h"
#include "Macaroni/Environment/Console.h"
#include "Macaroni/Exception.h"
#include "Macaroni/Environment/LuaEnvironment.h"
#include "Macaroni/Environment/Messages.h"
#include "Macaroni/IO/FileSet.h"
#include <iostream>
#include <sstream>
#include <Macaroni/VersionNo.h>

#ifdef MACARONI_COMPILE_TARGET_WINDOWS
	#include <tchar.h>
	#include "Macaroni/Platform/Windows/Strings.h"
	using Macaroni::Platform::Windows::NonWindowsString;
	using Macaroni::Platform::Windows::WindowsString;
#endif
#ifdef MACARONI_COMPILE_TARGET_LINUX
#endif


///////////////////////////////////////////////////////////////////////////////
//  Common
///////////////////////////////////////////////////////////////////////////////

using namespace Macaroni;
using namespace Macaroni;
using Macaroni::Build::CmdLine;
using Macaroni::Environment::Console;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Macaroni::IO::FileSet;
using Macaroni::Environment::Messages;

int generic_main(std::vector<MACARONI_VE_CONST std::string> & args)
{
	std::cout << MACARONI_FILE_DESCRIPTION << std::endl;	
	std::cout << "Version " << MACARONI_VERSION_STRING << 
		" built at " << BUILD_TIMESTAMP_LOCAL << std::endl;	
	std::cout << std::endl;

	Console output;

	CmdLine cmd(args, output);

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

///////////////////////////////////////////////////////////////////////////////
//  Windows
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_WINDOWS

void convert(std::wstring & original, std::string & rtnString)
{
	std::stringstream ss;
	for(unsigned int i = 0; i < original.size(); i ++)
	{
		ss << ((char)original[i]);
	}
	rtnString = ss.str();
}

/** I'm not sure if using the _TCHAR version of main is necessary. Maybe
 *  there's an easier way to achieve platform independence here. */
int _tmain(int argc, const _TCHAR * argv[])//_TCHAR* argv[])
{	
	std::vector<MACARONI_VE_CONST std::string> convertedArgs;
	for (int i = 0; i < argc; i ++)
	{
		NonWindowsString convertedString(argv[i]);
		convertedArgs.push_back(convertedString.get());
	}
	
	return generic_main(convertedArgs);
}

#endif // COMPILE_TARGET_WINDOWS

///////////////////////////////////////////////////////////////////////////////
//  Linux
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_LINUX

// This hack facilitates Macaroni::IO::Paths::GetExePath.
namespace Macaroni { namespace IO { 
std::string CURRENT_EXE_PATH; 
} };

int main(int argc, const char * argv[])
{	
	if (argc < 1)
	{
		std::cerr << "Less than one argument?! But how could that be?!!";
		return -500; // Negative 500 is super bad.
	}
	
	Macaroni::IO::CURRENT_EXE_PATH = argv[0];

	std::vector<MACARONI_VE_CONST std::string> convertedArgs;
	for (int i = 0; i < argc; i ++)
	{
		convertedArgs.push_back(argv[i]);
	}

	return generic_main(convertedArgs);
}
#endif // COMPILE_TARGET_LINUX

