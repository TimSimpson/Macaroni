#ifndef MACARONI_ENVIRONMENT_PROCESS_CPP
#define MACARONI_ENVIRONMENT_PROCESS_CPP

#include "Process.h"
#include "../Exception.h"
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib,"shell32.lib")
#include <sstream>
#include "../Platform/Windows/Strings.h"

namespace Macaroni { namespace Environment {

Process::Process(boost::filesystem::path fileName, const std::string & args, 
				 boost::filesystem::path workingDirectory,
		         const std::vector<const std::string> paths)
: args(args),
  fileName(fileName),
  paths(paths),
  workingDirectory(workingDirectory)
{
}

/** Appends all of the path variables to the PATH environment variable. */
void Process::mixinPathEnvVariables()
{
	const char * oldPathsValue = getenv("Path");
	if (oldPathsValue == nullptr)
	{
		throw Macaroni::Exception("Could not get the Path environment variable!");
	}
	std::stringstream ss;
	ss << oldPathsValue;
	for(unsigned int i = 0; i < paths.size(); i ++)
	{
		ss << ";" << paths[i];
	}

	_putenv_s("Path", ss.str().c_str());	
}

bool Process::Run(const Console & console)
{	
	
	std::stringstream ss;
	ss << "\"" << fileName.native_file_string() << "\" ";
	ss << " ";
	ss << args;

	mixinPathEnvVariables();
	
	console.WriteLine(ss.str());
	
	Macaroni::Platform::Windows::WindowsString wideFileName(fileName.native_file_string());
	Macaroni::Platform::Windows::WindowsString wideArgs(args);
	Macaroni::Platform::Windows::WindowsString wideWorkingDir(workingDirectory.string());

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

	if (CreateProcess(wideFileName.get(),
					  wideArgs.get(),
					  NULL,
					  NULL,
					  FALSE,
					  CREATE_DEFAULT_ERROR_MODE,
					  NULL,
					  wideWorkingDir.get(),
					  &startupInfo,
					  &processInfo) == FALSE) 
	{
		// BLAH!!
		console.WriteLine("An error occured when calling compiler.");
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
