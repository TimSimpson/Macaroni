#ifndef MACARONI_PLATFORM_WINDOWS_STRINGS_H
#define MACARONI_PLATFORM_WINDOWS_STRINGS_H

#include "../ME.h"
#include <string>
#include <windows.h>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

void ConvertToNormalString(std::wstring & original, std::string & rtnString)
{
	std::stringstream ss;	  
	for(unsigned int i = 0; i < original.size(); i ++)
	{
		ss << ((char)original[i]);
	}
	rtnString = ss.str();
}

class WideString
{
public:
	WideString(const std::string & original)
	{
		const char * orginalAsCStr = original.c_str();
		int lenA = lstrlenA(orginalAsCStr);
		int lenW = MultiByteToWideChar(CP_ACP, 0, orginalAsCStr, lenA, 0, 0);
		if (lenW > 0)
		{
			unicode = SysAllocStringLen(0, lenW);
			MultiByteToWideChar(CP_ACP, 0, orginalAsCStr, lenA, unicode, lenW);
		}
		else
		{
			throw Macaroni::Exception("Error converting normal string to Windows string.");
		}
	}
	
	~WideString() 
	{
		SysFreeString(unicode);	
	}
	
	inline BSTR get()
	{
		return unicode;
	}
private:
	BSTR unicode;
};

END_NAMESPACE

#endif
