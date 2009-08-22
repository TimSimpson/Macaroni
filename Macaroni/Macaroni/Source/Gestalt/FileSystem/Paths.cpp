#ifndef GESTALT_FILESYSTEM_PATHS_CPP
#define GESTALT_FILESYSTEM_PATHS_CPP

#include "Paths.h"
#include <boost/filesystem/operations.hpp>
#include <memory>
#include <sstream>
#include <stdlib.h>
#include <windows.h>

using boost::filesystem::path;

BEGIN_NAMESPACE2(Gestalt, FileSystem)

std::string createFromWChar(wchar_t * buffer)
{
	// Ued of a God-forsaken old char pointer is thanks to MB_CUR_MAX, which is 
	// actually a function and therefore prevents definition such as 
	// "char oldBuffer[MB_CUR_MAX]".
	// Maybe some solution such as use of auto_ptr variant might be possible, 
	// but right now I don't know how.
	char * oldBuffer;
	try
	{
		//std::basic_string<wchar_t> exePathW(buffer);
		int totalLength = wcslen(buffer);//exePathW.size();
		int expectedAsciiLength = totalLength + 1;
		oldBuffer = new char[expectedAsciiLength];//MB_CUR_MAX];				
		size_t convertedCount;

		wcstombs_s(&convertedCount, oldBuffer, expectedAsciiLength, buffer, totalLength);
		//wctomb_s(&convertedCount, oldBuffer, exePathW.size(), buffer);
		MACARONI_ASSERT(convertedCount == expectedAsciiLength,
						"Could not convert wide character string to classic char string.");
		return std::string(oldBuffer);
	}
	catch(...)
	{
		delete[] oldBuffer;
		throw; // Rethrows whatever we caught.
	}
	delete[] oldBuffer;
}

std::string Paths::GetExePath()
{ 
	wchar_t buffer[MAX_PATH + 1];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	return createFromWChar(buffer);
}

std::string Paths::GetExeDirectoryPath()
{ 
	path exePath(GetExePath());
	exePath.remove_leaf();
	std::string dirPath;
	dirPath = exePath.directory_string();
	return dirPath;/*
	char pathSeperators[2];
	pathSeperators[0] = '/';
	pathSeperators[1] = '\\';

	size_t index = exePath.find_last_of(pathSeperators, 0, 2);
	if (index == std::string::npos)
	{
		index = 0;	
	}

	std::string dirPath = exePath.substr(index);
	return dirPath;*/
}



END_NAMESPACE2

#endif

