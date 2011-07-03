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
#include <Macaroni/ME.h>
#ifndef MACARONI_ENVIRONMENT_PROCESS_CPP
#define MACARONI_ENVIRONMENT_PROCESS_CPP

#include "Process.h"
#include <boost/foreach.hpp>
#include <Macaroni/Platform/EnvironmentVariables.h>
#include "../Exception.h"
#include <memory>
#include <sstream>
#include "../Platform/Windows/Strings.h"

#ifdef MACARONI_COMPILE_TARGET_WINDOWS
#pragma comment(lib,"shell32.lib")
#include <windows.h>
#include <shellapi.h>
#endif

#ifdef MACARONI_COMPILE_TARGET_LINUX
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#endif

using Macaroni::Platform::EnvironmentVariable;
using Macaroni::Platform::EnvironmentVariables;

namespace Macaroni { namespace Environment {

///////////////////////////////////////////////////////////////////////////////
//  Common Implementation
///////////////////////////////////////////////////////////////////////////////

Process::Process(boost::optional<boost::filesystem::path> & fileName, 
	const std::vector<MACARONI_VE_CONST std::string> & args,
	boost::filesystem::path workingDirectory,
	const std::vector<MACARONI_VE_CONST std::string> paths,
	const std::vector<StringPair> & envVariables)
: args(args),
  envVariables(envVariables),
  fileName(fileName),
  paths(paths),
  workingDirectory(workingDirectory)
{
}

///////////////////////////////////////////////////////////////////////////////
//  Windows Implementation
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_WINDOWS

/** Appends all of the path variables to the PATH environment variable. */
void Process::mixinPathEnvVariables(EnvironmentVariables & vars)
{	
	EnvironmentVariable * pathEv = vars.Get("PATH");
		
	if (pathEv == nullptr)
	{
		throw Macaroni::Exception("Could not get the Path environment variable!");
	}
	std::stringstream ss;
	ss << pathEv->GetValue();
	for(unsigned int i = 0; i < paths.size(); i ++)
	{
		ss << ";" << paths[i];
	}

	pathEv->SetValue(ss.str());


	for (unsigned int j = 0; j < envVariables.size(); j ++)
	{
		StringPair pair = envVariables[j];		
		vars.Set(pair);
	}
}

bool Process::Run(const Console & console)
{	
	std::stringstream argStream;
	BOOST_FOREACH(MACARONI_VE_CONST std::string & arg, args)
	{
		argStream << arg;
	}
	std::string concatanatedArgs = argStream.str();

	std::stringstream ss;
	if (!!fileName)
	{
		ss << "\"" << fileName.get().string() << "\" ";
		ss << " ";
	}
	ss << concatanatedArgs;

	EnvironmentVariables vars;
	mixinPathEnvVariables(vars);
	
	console.WriteLine(ss.str());
	
	Macaroni::Platform::Windows::WindowsString wideFileName("");
	if (!!fileName)
	{
		wideFileName = fileName.get().string();	
	}	
	
	Macaroni::Platform::Windows::WindowsString wideArgs(concatanatedArgs);
	Macaroni::Platform::Windows::WindowsString wideWorkingDir(workingDirectory.string());

	std::auto_ptr<TCHAR> envBlock((TCHAR *) ::operator new(sizeof(TCHAR) * vars.GetNeededMemoryBlockSize()));
	vars.CopyToMemoryBlock(envBlock.get());

	/*LoadLibrary(TEXT("shell32.dll"));
	ShellExecute(GetDesktopWindow(),
		TEXT("open"),
		wideFileName.get(),
		wideArgs.get(),
		NULL,
		SW_SHOWNORMAL);*/
	STARTUPINFO         startupInfo;
    PROCESS_INFORMATION processInfo;
	memset(&startupInfo, 0, sizeof(startupInfo));
	memset(&processInfo, 0, sizeof(processInfo));

	DWORD creationFlags = 0;
	creationFlags = CREATE_DEFAULT_ERROR_MODE;
#ifdef UNICODE
	creationFlags |= CREATE_UNICODE_ENVIRONMENT;
#endif

	BOOL result;
	if (!fileName)
	{
		result = CreateProcess(NULL,
					  wideArgs.get(),
					  NULL,
					  NULL,
					  FALSE,
					  creationFlags,
					  (LPVOID) envBlock.get(),
					  wideWorkingDir.get(),
					  &startupInfo,
					  &processInfo);
	}
	else
	{
		result = CreateProcess(wideFileName.get(),
					  wideArgs.get(),
					  NULL,
					  NULL,
					  FALSE,
					  creationFlags,
					  (LPVOID) envBlock.get(),
					  wideWorkingDir.get(),
					  &startupInfo,
					  &processInfo);
	}

	if (result == FALSE) 
	{ 
		DWORD bug = ::GetLastError();
		LPVOID lpMsgBuf;	    
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			bug,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &lpMsgBuf,
			0, NULL );		
		console.WriteLine((LPCTSTR)lpMsgBuf);
		//MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

		LocalFree(lpMsgBuf);		
		
		// BLAH!!
		console.WriteLine("An error occurred when calling program.");
	}
	else 
	{
		//WaitForInputIdle(processInfo.hProcess, 2 * 60 * 1000);
		WaitForSingleObject(processInfo.hProcess, 2 * 60 * 1000);

	}

				


	//system(ss.str().c_str());
	
	return true;
}


#endif // MACARONI_COMPILE_TARGET_WINDOWS


///////////////////////////////////////////////////////////////////////////////
//  Linux Implementation
///////////////////////////////////////////////////////////////////////////////
#ifdef MACARONI_COMPILE_TARGET_LINUX

void Process::mixinPathEnvVariables(EnvironmentVariables & vars)
{
	MACARONI_THROW("This is needed only in Windows and should not be called "
		           "in Linux.");
}

// Allocates a new char array, copies src there, and returns a pointer to it.
char * copyStringToCharArray(const std::string & src)
{
	char * array = new char[src.length() + 1]; 
	strncpy(array, src.c_str(), src.length());
	return array;
}

bool Process::Run(const Console & console)
{	
	pid_t child = fork();
	if (child == -1)
	{
		MACARONI_THROW("Error forking child process!");
	}
	else if (child == 0)  // Child
	{ 		
		std::stringstream newPath;
		newPath << getenv("PATH");
		BOOST_FOREACH(MACARONI_VE_CONST std::string & path, this->paths)
		{
			newPath << path; 
		}	
		setenv("PATH", newPath.str().c_str(), 1);


		BOOST_FOREACH(const StringPair & envVar, this->envVariables)
		{
			setenv(envVar.Name.c_str(), envVar.Value.c_str(), 1);
		}	
		
		// None of the memory is ever reclaimed, but that doesn't matter
		// since execv replaces the process image.
		char **argv = new char* [this->args.size()+2];
		argv[0] = copyStringToCharArray(this->fileName->string());
		for (unsigned int i = 0; i < this->args.size(); i ++)
		{
			argv[i + 1] = copyStringToCharArray(this->args[i]);
		}
		argv[this->args.size()+2] = nullptr;

		int result = execv (fileName->string().c_str(), argv);
		return false;  // <-- Never reached.
	} 
	else
	{	
		int status;
		pid_t result = waitpid(child, &status, 0);
		if (result == -1) 
		{
			MACARONI_THROW("Failure waiting for child process.");
		}
		if (WIFEXITED(status))
		{
			return true;
		}
		else
		{
			return false;
		}		
	}
}

#endif // MACARONI_COMPILE_TARGET_LINUX

///////////////////////////////////////////////////////////////////////////////
//  Common
///////////////////////////////////////////////////////////////////////////////

} }

#endif // file compilation guard