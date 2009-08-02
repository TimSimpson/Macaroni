#ifndef MACARONI_MODELINCONSISTENCYEXCEPTION_H
#define MACARONI_MODELINCONSISTENCYEXCEPTION_H

#include "../ME.h"
#include "Reason.lh"
#include <string>
#include "../Model/SourceException.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents some rule or principal which client code uses to create or alter
 * the Model. */
class ModelInconsistencyException : public SourceException
{
public:
	ModelInconsistencyException(SourcePtr source, const std::string & msg)
		: SourceException(source, msg)
	{
	}

	/** Used when an new reason for a change is inconsistent with an existing
	 * reason. */
	ModelInconsistencyException(ReasonPtr originalReason, ReasonPtr newReason, 
								const std::string & additionalInfo);

	const char * GetMessage() const
	{
		return msg.c_str();
	}
private:
	std::string msg;
};

END_NAMESPACE2

#endif