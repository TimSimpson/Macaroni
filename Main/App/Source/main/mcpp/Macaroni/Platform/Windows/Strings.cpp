#include <Macaroni/ME.h>
#ifdef MACARONI_COMPILE_TARGET_WINDOWS

#ifndef MACARONI_PLATFORM_WINDOWS_STRINGS_CPP
#define MACARONI_PLATFORM_WINDOWS_STRINGS_CPP

#include "Strings.h"
#include "../../Exception.h"
#include <windows.h>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

#ifdef _UNICODE

WindowsString::WindowsString(const std::string & original)
{

	const char * orginalAsCStr = original.c_str();
	int lenA = lstrlenA(orginalAsCStr);
	int lenW = MultiByteToWideChar(CP_ACP, 0, orginalAsCStr, lenA, 0, 0);
	if (lenW > 0)
	{
		//windowsString = (TCHAR *) ::operator new(sizeof(TCHAR) * lenW + 1); //SysAllocStringLen(0, lenW);
		windowsString = SysAllocStringLen(0, lenW);
		MultiByteToWideChar(CP_ACP, 0, orginalAsCStr, lenA, windowsString, lenW);
	}
	else
	{
		throw Macaroni::Exception("Error converting normal string to Windows string.");
	}	
}

WindowsString::~WindowsString() 
{
	SysFreeString(windowsString);	
	//delete windowsString;
}
	
	

NonWindowsString::NonWindowsString(const wchar_t * wStr)
	:str(createFromWChar(wStr))
{
}

std::string NonWindowsString::createFromWChar(const wchar_t * buffer)
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

#else // if ascii

WindowsString::WindowsString(const std::string & original)
	:str(NULL)
{
	str = new char[original.size() + 1];
	strncpy(str, original.c_str(), original.size());
	str[original.size()] = '\0';
}
	
WindowsString::~WindowsString() 
{
	delete[] str;
}

NonWindowsString::NonWindowsString(const char * cStr)
	:str(cStr)
{
}
	
#endif // not unicode

END_NAMESPACE

#endif

#endif // MACARONI_COMPILE_TARGET_WINDOWS