#ifndef MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLE_H
#define MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLE_H

#include "../../ME.h"
//#include "Strings.h"
#include <string>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

class EnvironmentVariable
{
public:
	EnvironmentVariable(const std::string & definitionString);

	EnvironmentVariable(const std::string & name, const std::string & value);

	inline const std::string & GetName() const 
	{
		return name;
	}

	static std::string GetNameFromDefinition(const std::string & def);

	inline const std::string & GetValue() const
	{
		return value;
	}

	static std::string GetValueFromDefinition(const std::string & def);

	static const std::string LowerCaseString(std::string target);

	inline void SetValue(const std::string & v) 
	{
		value = v;
	}
private:	
	std::string name;
	std::string value;	
};

END_NAMESPACE

#endif
