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
#ifndef MACARONI_PLATFORM_FILETIME_CPP
#define MACARONI_PLATFORM_FILETIME_CPP

#include <Macaroni/Exception.h>
#include "FileTime.h"
#include <sstream>

#ifdef MACARONI_COMPILE_TARGET_WINDOWS
#include <windows.h>
#include <Macaroni/Platform/Windows/Strings.h>
#endif

#ifdef MACARONI_COMPILE_TARGET_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#endif

using boost::filesystem::path ;

BEGIN_NAMESPACE2(Macaroni, Platform)

#ifdef MACARONI_COMPILE_TARGET_WINDOWS

using Macaroni::Platform::Windows::NonWindowsString;
using Macaroni::Platform::Windows::WindowsString;

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
#endif // End Windows

#ifdef MACARONI_COMPILE_TARGET_LINUX

time_t modifiedTime(const char * filePath)
{
	struct stat fileInfo;
	if (stat(filePath, &fileInfo) == -1) {
		std::stringstream ss;
		ss << "Error getting modified time (via stat) of file \"" 
		   << filePath << "\".";
		throw Macaroni::Exception(ss.str().c_str());
	}
	return fileInfo.st_mtime;
}

bool FileTime::File1IsOlderThanFile2(boost::filesystem::path & one,
									 boost::filesystem::path & two)
{ 
	time_t f1 = modifiedTime(one.string().c_str());
	time_t f2 = modifiedTime(two.string().c_str());
	return f1 > f2;
}

#endif

END_NAMESPACE2

#endif
