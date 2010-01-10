#ifndef MACARONI_MODEL_SOURCEEXCEPTION_H
#define MACARONI_MODEL_SOURCEEXCEPTION_H

#include "../ME.h"
#include <string>
#include "Source.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Any exception which occurs because the Source is incorrect. */
class SourceException : public std::exception
{
public:
	SourceException(SourcePtr source, const std::string & msg)
		: source(source), msg(msg)
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

	virtual const char * what() const throw()
	{
		return GetMessage().c_str();
	}

private:
	std::string msg;
	SourcePtr source;
};

END_NAMESPACE2

#endif
