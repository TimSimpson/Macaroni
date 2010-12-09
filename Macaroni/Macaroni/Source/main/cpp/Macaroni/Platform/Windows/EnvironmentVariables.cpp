#ifndef MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLES_CPP
#define MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLES_CPP

#include "../../ME.h"
#include "EnvironmentVariables.h"
#include "../../Exception.h"
#include <memory>
#include <sstream>
#include <tchar.h>
#include <boost/algorithm/string.hpp>   
#include "Strings.h"
#include <windows.h>

using Macaroni::Platform::Windows::NonWindowsString;
using Macaroni::Environment::StringPair;
using Macaroni::Platform::Windows::WindowsString;

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

EnvironmentVariables::EnvironmentVariables()
:vars()
{
	LPTCH orig = ::GetEnvironmentStrings();
	try
	{
		if (orig == NULL)
		{
			throw Macaroni::Exception("Could not grab environment strings.");
		}

		LPTSTR blockAsLpstr = (LPTSTR) orig;
		while(*blockAsLpstr)
		{
			size_t len = lstrlen(blockAsLpstr);
			std::auto_ptr<TCHAR> buffer((TCHAR *) (::operator new(sizeof(TCHAR) * (len + 1))));
			//::StringCchCopy(buffer.get(), len, blockAsLpstr);
			lstrcpyn(buffer.get(), blockAsLpstr, len + 1);
			//lstrncpy(buffer.get(), blockAsLpstr, len);
			(buffer.get())[len]='\0';
			NonWindowsString winStr(buffer.get());
			std::string normal = winStr.get();
			vars.push_back(EnvironmentVariable(normal));
			blockAsLpstr += (len + 1);
		}
		::FreeEnvironmentStrings((__in LPTCH) orig);
	} 
	catch(std::exception & ex) 
	{	
		::FreeEnvironmentStrings((__in LPTCH) orig);
		throw ex;
	}	

	//TCHAR * blockItr = (TCHAR *) orig;
	//while(blockItr !='\0') // This memory we get back from Windows ends with \0\0
	//{
	//	TCHAR * varItr;
	//	for (varItr = blockItr; varItr != '\0'; varItr ++)
	//	{			
	//	}
	//	NonWindowsString fullWindowsStr(blockItr);
	//	std::string fullStr = fullWindowsStr.get();
	//	vars.push_back(EnvironmentVariable(fullStr, ""));
	//	blockItr ++;
	//}

}

EnvironmentVariables::~EnvironmentVariables()
{	
}

void EnvironmentVariables::CopyToMemoryBlock(void * vBlock)
{
	TCHAR * block = (TCHAR *) vBlock;
	for (unsigned int i = 0; i < vars.size(); i ++)
	{
		EnvironmentVariable & v = vars[i];
		std::stringstream ss;
		ss << v.GetName() << "=" << v.GetValue();
		std::string def = ss.str();

		Macaroni::Platform::Windows::WindowsString winStr(def);
		lstrcpyn(block, winStr.get(), def.size() + 1);
		block[def.size() + 1] = '\0';
		block += (def.size() + 1);
	}
	block[0] = '\0';

	// NOW CHECK AND SEE WHY IT WON'T WORK.
	std::vector<EnvironmentVariable> newVars;

	LPTSTR blockAsLpstr = (LPTSTR) vBlock;
	while(*blockAsLpstr)
	{
		size_t len = lstrlen(blockAsLpstr);
		std::auto_ptr<TCHAR> buffer((TCHAR *) (::operator new(sizeof(TCHAR) * (len + 1))));
		//::StringCchCopy(buffer.get(), len, blockAsLpstr);
		lstrcpyn(buffer.get(), blockAsLpstr, len + 1);
		//lstrncpy(buffer.get(), blockAsLpstr, len);
		(buffer.get())[len]='\0';
		try{
			NonWindowsString winStr(buffer.get());		
			std::string normal = winStr.get();
			newVars.push_back(EnvironmentVariable(normal));
			blockAsLpstr += (len + 1);
		} catch(std::exception ex) {
			throw ex;
		}
	}	
	

	for (unsigned int i = 0; i < newVars.size(); i ++)
	{
		EnvironmentVariable & a = vars[i];
		EnvironmentVariable & b = newVars[i];
		if (a.GetName() != b.GetName() ||
			a.GetValue() != b.GetValue())
		{
			int five = 5;
		}
	}
}

EnvironmentVariable * EnvironmentVariables::Get(const std::string & name)
{
	std::string low = name;
	boost::to_lower(low);
	for (unsigned int i = 0; i < vars.size(); i ++)
	{
		if (vars[i].GetName() == low)
		{
			return &(vars[i]);
		}
	}
	return nullptr;
}


size_t EnvironmentVariables::GetNeededMemoryBlockSize()
{
	size_t total = 0;
	for (unsigned int i = 0; i < vars.size(); i ++)
	{
		EnvironmentVariable & v = vars[i];
		total += (v.GetName().size() + 1 + v.GetValue().size() + 1);
	}
	total ++;
	return total;
}

void EnvironmentVariables::Set(Macaroni::Environment::StringPair & pair)
{
	std::string low = pair.Name;
	boost::to_lower(low);
	for (unsigned int i = 0; i < vars.size(); i ++)
	{
		EnvironmentVariable & v = vars[i];
		if (v.GetName() == low)
		{
			v.SetValue(pair.Value);
			return;
		}
	}
	vars.push_back(EnvironmentVariable(pair.Name, pair.Value));
}

END_NAMESPACE

#endif
