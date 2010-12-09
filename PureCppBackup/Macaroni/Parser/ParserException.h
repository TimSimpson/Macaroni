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
		: source(source), msg(msg), throwLocation(throwLocation)
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
