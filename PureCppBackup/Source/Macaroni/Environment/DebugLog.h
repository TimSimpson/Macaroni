#ifndef MACARONI_ENVIRONMENT_DEBUGLOG_H
#define MACARONI_ENVIRONMENT_DEBUGLOG_H

#include "../ME.h"
#include <iostream>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Environment)

#define DEBUGLOG_WRITE(message) Macaroni::Environment::DebugLog::Write(__FILE__, __LINE__, message);

class DebugLog
{
public:
	
	static void Write(const char * file, int line, const char * message);

	static void Write(const char * file, int line, std::string & message);

};

END_NAMESPACE2

#endif
