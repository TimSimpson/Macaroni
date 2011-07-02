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
#ifndef MACARONI_MODELINCONSISTENCYEXCEPTION_CPP
#define MACARONI_MODELINCONSISTENCYEXCEPTION_CPP

#include "ModelInconsistencyException.h"
#include <Macaroni/Model/Reason.h>
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
