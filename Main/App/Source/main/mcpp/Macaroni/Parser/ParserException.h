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

#include "../ME.h"
#include <exception>
#include <string>
#include "ParserExceptionPtr.h"
#include "../Model/SourcePtr.h"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Any exception which occurs within the Parser. */
class ParserException : public std::exception
{
public:
	ParserException(Model::SourcePtr source, const std::string & msg, const std::string & throwLocation = "")
		: msg(msg), source(source), throwLocation(throwLocation)
	{
	}

	virtual ~ParserException() throw()
	{	
	}
	
	Model::SourcePtr GetSource() const  
	{
		return source;
	}

	const char * GetMessage() const
	{
		return msg.c_str(); 
	}

	const char * GetThrowLocation() const
	{
		return throwLocation.c_str();
	}

	virtual const char * what() const throw()
	{
		return GetMessage(); 
	}

private: 
	std::string msg; 
	Model::SourcePtr source;
	std::string throwLocation;
};

END_NAMESPACE2

#endif
