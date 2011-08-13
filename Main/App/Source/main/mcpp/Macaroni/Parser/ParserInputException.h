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
#ifndef MACARONI_PARSER_PARSEREXCEPTION_H
#define MACARONI_PARSER_PARSEREXCEPTION_H

#include "../Model/SourceException.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Any exception which occurs because the Source is incorrect. */
class ParserInputException : public SourceException
{
public:
	ParserInputException(SourcePtr source, const std::string & msg)
		: source(source), msg(msg)
	{
	}

	const char * GetMessage() const
	{
		return msg.c_str();
	}

	SourcePtr GetSource() const
	{
		return source;
	}

private:
	std::string msg;
	SourcePtr source;
};

END_NAMESPACE2

#endif