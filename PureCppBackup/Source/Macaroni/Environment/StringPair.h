#ifndef MACARONI_ENVIRONMENT_STRINGPAIR_H
#define MACARONI_ENVIRONMENT_STRINGPAIR_H

#include "../ME.h"

#include <string>

BEGIN_NAMESPACE2(Macaroni, Environment)

struct StringPair
{
	std::string Name;
	std::string Value;
};

END_NAMESPACE2

#endif
