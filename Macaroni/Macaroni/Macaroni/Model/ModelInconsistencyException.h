#ifndef MACARONI_MODELINCONSISTENCYEXCEPTION_H
#define MACARONI_MODELINCONSISTENCYEXCEPTION_H

#include "../ME.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents some rule or principal which client code uses to create or alter
 * the Model. */
class ModelInconsistencyException
{
public:
	ModelInconsistencyException(const std::string & msg)
		: msg(msg)
	{
	}

	const char * GetMessage() const
	{
		return msg.c_str();
	}
private:
	std::string msg;
};

END_NAMESPACE2

#endif
