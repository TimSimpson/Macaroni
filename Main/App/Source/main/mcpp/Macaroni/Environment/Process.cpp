#ifndef MACARONI_ENVIRONMENT_PROCESS_CPP
#define MACARONI_ENVIRONMENT_PROCESS_CPP

#include "Process.h"
#include "../Platform/Windows/EnvironmentVariables.h"
#include "../Exception.h"
#include <memory>
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib,"shell32.lib")
#include <sstream>
#include "../Platform/Windows/Strings.h"

using Macaroni::Platform::Windows::EnvironmentVariable;
using Macaroni::Platform::Windows::EnvironmentVariables;

namespace Macaroni { namespace Environment {

Process::Process(boost::optional<boost::filesystem::path> & fileName, 
				 const std::string & a, 
				 boost::filesystem::path workingDirectory,
		         const std::vector<const std::string> paths,
				 const std::vector<StringPair> & envVariables)
: args(a),
  envVariables(envVariables),
  fileName(fileName),
  paths(paths),
  workingDirectory(workingDirectory)
{
}

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
	
	std::stringstream ss;
	if (!!fileName)
	{
		ss << "\"" << fileName.get().native_file_string() << "\" ";
		ss << " ";
	}
	ss << args;

	EnvironmentVariables vars;
	mixinPathEnvVariables(vars);
	
	console.WriteLine(ss.str());
	
	Macaroni::Platform::Windows::WindowsString wideFileName("");
	if (!!fileName)
	{
		wideFileName = fileName.get().native_file_string();	
	}	
	Macaroni::Platform::Windows::WindowsString wideArgs(args);
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
		console.WriteLine("An error occured when calling program.");
	}
	else 
	{
		//WaitForInputIdle(processInfo.hProcess, 2 * 60 * 1000);
		WaitForSingleObject(processInfo.hProcess, 2 * 60 * 1000);

	}

				


	//system(ss.str().c_str());
	
	return true;
}

} }

#endif
