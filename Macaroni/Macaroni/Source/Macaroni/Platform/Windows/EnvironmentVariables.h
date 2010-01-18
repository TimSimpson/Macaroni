#ifndef MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLES_H
#define MACARONI_PLATFORM_WINDOWS_ENVIRONMENTVARIABLES_H

#include "../../ME.h"
#include "EnvironmentVariable.h"
//#include "Strings.h"
#include "../../Environment/StringPair.h"
#include <vector>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

class EnvironmentVariables
{
public:
	EnvironmentVariables();
	~EnvironmentVariables();

	void CopyToMemoryBlock(void *);

	EnvironmentVariable * Get(const std::string & name);

	size_t GetNeededMemoryBlockSize();
	
	void Set(Macaroni::Environment::StringPair & pair);

private:
	std::vector<EnvironmentVariable> vars;
};

END_NAMESPACE

#endif
