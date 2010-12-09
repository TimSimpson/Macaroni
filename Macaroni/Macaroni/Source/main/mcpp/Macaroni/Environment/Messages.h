#ifndef MACARONI_ENVIRONMENT_MESSAGES_H
#define MACARONI_ENVIRONMENT_MESSAGES_H

#include "../ME.h"

#include <map>
#include <string>

BEGIN_NAMESPACE2(Macaroni, Environment)

class Messages
{
public:
	static std::string Get(const char * id);

	static std::string Get(const char * id, int value);

	template<typename ObjectType>
	static std::string Get(const char * id, ObjectType value);
	
	// Initializes all Messages.
	static void Init();
private:
	Messages(const char * filePath);
	
	void add(std::string key, std::string content);
	
	std::map<std::string, std::string> bag;

	std::string get(const char * id);

	static Messages & getInstance();
};


END_NAMESPACE2

#endif
