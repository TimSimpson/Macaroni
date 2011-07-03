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
#ifndef MACARONI_IO_PATHS_CPP
#define MACARONI_IO_PATHS_CPP

#include <Macaroni/ME.h>
#include "Paths.h"
#include <boost/filesystem/operations.hpp>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <Macaroni/Platform/Windows/Strings.h>



#ifdef MACARONI_COMPILE_TARGET_WINDOWS
	#include <tchar.h>
	#include <windows.h>

	#define HOME_DIRECTORY_ENV_VAR_NAME "USERPROFILE"
#endif

#ifdef MACARONI_COMPILE_TARGET_LINUX
	#define HOME_DIRECTORY_ENV_VAR_NAME "HOME"
#endif

///////////////////////////////////////////////////////////////////////////////
//      Common Implementation
///////////////////////////////////////////////////////////////////////////////
using boost::filesystem::path;

BEGIN_NAMESPACE2(Macaroni, IO)

std::string Paths::GetExeDirectoryPath()
{ 
	path exePath0(GetExePath());
	path exePath = boost::filesystem::system_complete(exePath0);
	exePath.remove_leaf();
	std::string dirPath;
	dirPath = exePath.string();
	return dirPath;
}

std::string Paths::GetUserPath() 
{
	const char * p =  getenv(HOME_DIRECTORY_ENV_VAR_NAME);
	if (p == nullptr)
	{
		MACARONI_THROW("Error getting path to home directory!");
	}
	std::string userProfile(p);
	std::string userPath = userProfile + "\\Macaroni";
	return userPath;
}

END_NAMESPACE2


///////////////////////////////////////////////////////////////////////////////
//      Windows Implementation
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_WINDOWS

using Macaroni::Platform::Windows::NonWindowsString;
using Macaroni::Platform::Windows::WindowsString;

BEGIN_NAMESPACE2(Macaroni, IO)

std::string Paths::GetExePath()
{ 
	TCHAR buffer[MAX_PATH + 1];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	NonWindowsString str(buffer);
	return str.get();
}

END_NAMESPACE2

#endif // Windows

///////////////////////////////////////////////////////////////////////////////
//      Linux Implementation
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_LINUX

BEGIN_NAMESPACE2(Macaroni, IO)

/** This is the saved value of argv[0] in Linux... its super dangerous, and
 *  super awesome!
 */
extern std::string CURRENT_EXE_PATH; 

std::string Paths::GetExePath()
{ 
	return CURRENT_EXE_PATH;
}

END_NAMESPACE2

#endif // Linux

#endif 
