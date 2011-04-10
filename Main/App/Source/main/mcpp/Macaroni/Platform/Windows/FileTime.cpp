#ifndef MACARONI_PLATFORM_WINDOWS_FILETIME_CPP
#define MACARONI_PLATFORM_WINDOWS_FILETIME_CPP

#include "../../Exception.h"
#include "FileTime.h"
#include <windows.h>
#include "Strings.h"
#include <sstream>

using boost::filesystem::path ;

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

struct FT
{
	FILETIME access;
	FILETIME create;	
	FILETIME write;

	FT(path p)
	{
		WindowsString wStr(p.string());

		HANDLE h = ::CreateFile(wStr.get(),
							 GENERIC_READ,
							 FILE_SHARE_READ,
							 NULL, // security
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL,
							 NULL);
		if (h == INVALID_HANDLE_VALUE)
		{
			DWORD err = ::GetLastError();
			std::stringstream ss;
			ss << "Could not open file \"" << p.string() << "\" to get time.";
			throw Macaroni::Exception(ss.str().c_str());
		}

		try
		{
			if (::GetFileTime(h, &access, &create, &write) == 0)
			{
				std::stringstream ss;
				ss << "Could not get file time for file \"" 
					<< p.string() << "\" to get time.";
				throw Macaroni::Exception(ss.str().c_str());
			}
		} 
		catch(std::exception & ex)
		{
			::CloseHandle(h);
			throw ex;
		}
		::CloseHandle(h);
	};
};

bool FileTime::File1IsOlderThanFile2(boost::filesystem::path & one,
									  boost::filesystem::path & two)
{ 
	FT f1(one);
	FT f2(two);
	SYSTEMTIME t1;
	SYSTEMTIME t2;
	::FileTimeToSystemTime(&f1.write, &t1);
	::FileTimeToSystemTime(&f2.write, &t2);

	if (t1.wYear != t2.wYear) 
	{
		return t1.wYear < t2.wYear;
	}
	if (t1.wMonth != t2.wMonth)
	{
		return t1.wMonth < t2.wMonth;
	}
	if (t1.wDay != t2.wDay)
	{
		return t1.wDay < t2.wDay;
	}
	if (t1.wHour != t2.wHour)
	{
		return t1.wHour < t2.wHour;
	}
	if (t1.wMinute != t2.wMinute)
	{
		return t1.wMinute < t2.wMinute;
	}
	if (t1.wSecond != t2.wSecond)
	{
		return t1.wSecond < t2.wSecond;
	}
	if (t1.wMilliseconds != t2.wMilliseconds)
	{
		return t1.wMilliseconds < t2.wMilliseconds;
	}
	return false;
}

END_NAMESPACE

#endif
