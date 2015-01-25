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
#ifndef MACARONI_MODEL_SOURCEEXCEPTION_H
#define MACARONI_MODEL_SOURCEEXCEPTION_H

#include "../ME.h"
#include <string>
#include "SourcePtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Any exception which occurs because the Source is incorrect. */
class SourceException : public std::exception
{
public:
	SourceException(SourcePtr source, const std::string & msg)
		: msg(msg), source(source)
	{
	}

	~SourceException() BOOST_NOEXCEPT
	{
	}

	const std::string & GetMessage() const
	{
		return msg;
	}

	SourcePtr GetSource() const
	{
		return source;
	}

	const char * what() const BOOST_NOEXCEPT override
	{
		return GetMessage().c_str();
	}

private:
	std::string msg;
	SourcePtr source;
};

END_NAMESPACE2

#endif
