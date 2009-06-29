#ifndef MACARONI_PARSER_PARSEREXCEPTION_H
#define MACARONI_PARSER_PARSEREXCEPTION_H

#include "../ME.h"
#include <string>
#include "../Model/Source.lh"

BEGIN_NAMESPACE2(Macaroni, Parser)

/** Any exception which occurs within the Parser. */
class ParserException 
{
public:
	ParserException(Model::SourcePtr source, const std::string & msg)
		: source(source), msg(msg)
	{
	}

	const char * GetMessage() const
	{
		return msg.c_str(); 
	}

	Model::SourcePtr GetSource() const  
	{
		return source;
	}

private: 
	std::string msg; 
	Model::SourcePtr source;
};

END_NAMESPACE2

#endif
