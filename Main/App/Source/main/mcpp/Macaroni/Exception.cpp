#ifndef MACARONI_EXCEPTION_CPP
#define MACARONI_EXCEPTION_CPP

#include "Exception.h"
#include <sstream>

namespace Macaroni{  

void ThrowMacaroniException(const char * file, int line, const char * message)
{
	std::stringstream ss;
	ss << file << ", line " << line;
	throw Exception(ss.str().c_str(), message);
};

void ThrowMacaroniException(const char * file, int line, const std::string & message)
{
	std::stringstream ss;
	ss << file << ", line " << line;
	throw Exception(ss.str().c_str(), message.c_str());
};
 
} // end of namespace


#endif