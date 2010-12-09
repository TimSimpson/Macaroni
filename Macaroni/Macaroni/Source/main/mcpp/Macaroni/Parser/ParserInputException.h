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
