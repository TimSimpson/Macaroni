#ifndef MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLE_CPP
#define MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLE_CPP

#include "EnvironmentVariable.h"
#include <boost/algorithm/string.hpp>   

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

EnvironmentVariable::EnvironmentVariable(const std::string & definitionString)
		: name(LowerCaseString(GetNameFromDefinition(definitionString))),
		  value(GetValueFromDefinition(definitionString))
{
}

EnvironmentVariable::EnvironmentVariable(const std::string & name, const std::string & value)
	: name(LowerCaseString(name)), value(value)
{
}	

std::string EnvironmentVariable::GetNameFromDefinition(const std::string & def)
{
	size_t index = def.find_first_of('=');
	if (index == 0)
	{
		index = def.find_first_of('=', 1);
	} else if (index < 0) 
	{
		index = def.size();
	}
	return def.substr(0, index);
}

std::string EnvironmentVariable::GetValueFromDefinition(const std::string & def)
{
	size_t index = def.find_first_of('=');
	if (index == 0)
	{
		index = def.find_first_of('=', 1);
	} else if (index < 0)
	{
		return "";
	}
	index ++;
	return def.substr(index, def.size() - index);
}

const std::string EnvironmentVariable::LowerCaseString(std::string target)
{
	boost::to_lower(target);
	return target;
}

END_NAMESPACE

#endif
