#ifndef MACARONI_ENVIRONMENT_DEBUGLOG_CPP
#define MACARONI_ENVIRONMENT_DEBUGLOG_CPP

#include "DebugLog.h"

BEGIN_NAMESPACE2(Macaroni, Environment)

void DebugLog::Write(const char * file, int line, const char * message)
{
	//std::cerr << "|DEBUG| " << file << "(" << line << ") : " << message << "\n";
}

void DebugLog::Write(const char * file, int line, std::string & message)
{
	//std::cerr << "|DEBUG| " << file << "(" << line << ") : " << message << "\n";
}

END_NAMESPACE2

#endif
