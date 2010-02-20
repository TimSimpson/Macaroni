#ifndef MACARONI_ENVIRONMENT_DEBUGLOG_CPP
#define MACARONI_ENVIRONMENT_DEBUGLOG_CPP

#include "DebugLog.h"
extern "C" {
	#include "DebugLogC.h"
}

BEGIN_NAMESPACE2(Macaroni, Environment)

void DebugLog::Write(const char * file, int line, const char * message)
{
	FILE * log = DebugGetLogFile();
	fprintf(log, "|DEBUG| %s ( %i ) : %s \n", file, line, message);	
	/*std::cerr << "|DEBUG| " << file << "(" << line << ") : " << message << "\n";*/
}

void DebugLog::Write(const char * file, int line, std::string & message)
{
	Write(file, line, message.c_str());
	//std::cerr << "|DEBUG| " << file << "(" << line << ") : " << message << "\n";
}

END_NAMESPACE2

#endif
