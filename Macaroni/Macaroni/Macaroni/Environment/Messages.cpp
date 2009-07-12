#ifndef MACARONI_ENVIRONMENT_MESSAGES_CPP
#define MACARONI_ENVIRONMENT_MESSAGES_CPP

#include "Messages.h"

#include <fstream>
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Environment)

namespace
{
	template<typename ObjectType>
	void replace(std::string & str, int index, ObjectType obj)
	{		
		std::stringstream marker;
		marker << "{" << index << "}";
		int indexOf = str.find_first_of(marker.str());		
		if (indexOf != std::string::npos)
		{
			std::stringstream newValue;
			newValue << obj;
			str.replace(indexOf, indexOf + marker.str().size(), newValue.str());
		}
	}
}

Messages::Messages(const char * filePath)
:bag()
{
	std::ifstream file;
	file.open("Messages.txt");
	MACARONI_ASSERT(!(!file), "Unable to open Messages file!");
	std::string next;
		
 	while(!std::getline(file, next).eof())
	{
		int index = next.find_first_of("=");
		if (next.size() > 0 && next[0] != '#')
		{
			std::string key = next.substr(0, index);
			std::string content = next.substr(index + 1, next.size() - index);
			add(key, content);
		}		
	}
	file.close();
}

void Messages::add(std::string key, std::string content)
{
	bag[key] = content;
}


std::string Messages::Get(const char * id)
{	
	return getInstance().get(id);
}

std::string Messages::Get(const char * id, int var)
{	
	std::string msg(Get(id));
	replace(msg, 0, var);
	return msg;
}


std::string Messages::get(const char * id)
{
	std::string key(id);
	return bag[key];
	/*if (bag.count(key) > 0)
	{
		return bag[key];
	}
	return std::string("~Mysterious Message of Macaroni~");*/
}

Messages & Messages::getInstance()
{
	static Messages * instance = nullptr;
	if (instance == nullptr)
	{
		instance = new Messages("Messages.txt");
	}	
	return dynamic_cast<Messages &>(*instance);
}



END_NAMESPACE2

#endif
