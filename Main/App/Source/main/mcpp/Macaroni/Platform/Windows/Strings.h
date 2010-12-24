#ifndef MACARONI_PLATFORM_WINDOWS_STRINGS_H
#define MACARONI_PLATFORM_WINDOWS_STRINGS_H

#include "../../ME.h"
#include <string>
#include <tchar.h>
#include <windows.h>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)
//
//void ConvertToNormalString(std::wstring & original, std::string & rtnString)
//{
//	std::stringstream ss;	  
//	for(unsigned int i = 0; i < original.size(); i ++)
//	{
//		ss << ((char)original[i]);
//	}
//	rtnString = ss.str();
//}

#ifdef _UNICODE

class WindowsString
{
public:
	WindowsString(const std::string & original);
	
	~WindowsString() ;

	inline TCHAR  * get()
	{
		return windowsString;
	}
private:
	wchar_t * windowsString;
};

class NonWindowsString
{
public:
	NonWindowsString(const wchar_t * wStr);
	inline std::string & get()
	{
		return str;
	}
private:
	
	std::string createFromWChar(const wchar_t * buffer);
	std::string str;
};
#else // if ascii

class WindowsString
{
public:
	WindowsString(const std::string & original);
	~WindowsString() ;
	
	inline char * get()
	{
		return str;
	}
private:
	char * str;
};

class NonWindowsString
{
public:
	NonWindowsString(const char * cStr);
	inline std::string & get()
	{
		return str;
	}
private:
	std::string str;
};
#endif // not unicode

END_NAMESPACE

#endif
