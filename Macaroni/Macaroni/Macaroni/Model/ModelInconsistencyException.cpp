#ifndef MACARONI_MODELINCONSISTENCYEXCEPTION_CPP
#define MACARONI_MODELINCONSISTENCYEXCEPTION_CPP

#include "ModelInconsistencyException.h"
#include "Reason.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

namespace {

	static std::string createMessage(ReasonPtr originalReason, 
									 ReasonPtr newReason,
									 const std::string & additionalInfo)
	{
		std::stringstream ss;
		ss << "Conflict exists between two sources. "
			<< "Existing reasoning [" << originalReason->ToString() 
			<< "] is inconsistent with newer reasoning [" 
			<< newReason->ToString() 
			<< "]. Additional info: " << additionalInfo;
		return ss.str();
	}

}
ModelInconsistencyException::ModelInconsistencyException(
	ReasonPtr originalReason, ReasonPtr newReason, 
	const std::string & additionalInfo)
	: SourceException(newReason->GetSource(), 
					  createMessage(originalReason, newReason, additionalInfo))
{
}

END_NAMESPACE2

#endif
