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
#ifndef MACARONI_MODELINCONSISTENCYEXCEPTION_H
#define MACARONI_MODELINCONSISTENCYEXCEPTION_H

#include "../ME.h"
#include "ReasonPtr.h"
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
	
private:	
};

END_NAMESPACE2

#endif
