#ifndef MACARONI_PARSEREXCEPTION_H
#define MACARONI_PARSEREXCEPTION_H

#include "../Exception.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class ParserException
{
public:
	ParserException(std::string msg) : msg(msg) {};
private:
	std::string msg;
};

END_NAMESPACE2

#endif
