#ifndef MACARONI_PARSER_PARSEREXCEPTION_H
#define MACARONI_PARSER_PARSEREXCEPTION_H

#include "../ME.h"
#include <exception>
#include <string>
#include "ParserException.lh"
#include "../Model/Source.lh"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Any exception which occurs within the Parser. */
class ParserException : public std::exception
{
public:
	ParserException(Model::SourcePtr source, const std::string & msg)
		: source(source), msg(msg)
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

	virtual const char * what() const throw()
	{
		return GetMessage(); 
	}

private: 
	std::string msg; 
	Model::SourcePtr source;
};

END_NAMESPACE2

#endif
