#ifndef MACARONI_ENVIRONMENT_MESSAGES_CPP
#define MACARONI_ENVIRONMENT_MESSAGES_CPP

#include "Messages.h"

#include <fstream>
#include "../IO/Paths.h"
#include <sstream>

using Macaroni::IO::Paths;

BEGIN_NAMESPACE2(Macaroni, Environment)

namespace
{
	template<typename ObjectType>
	void replace(std::string & str, unsigned int index, ObjectType obj)
	{		
		std::stringstream marker;
		marker << "{" << index << "}";
		unsigned int indexOf = str.find_first_of(marker.str());		
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
	std::string exePath = Paths::GetExeDirectoryPath();
	std::ifstream file;
	std::stringstream finalFilePath;
	finalFilePath << exePath << "Messages.txt";
	file.open(finalFilePath.str().c_str());
	if (!!file) 
	{
		std::string next;
			
 		do
		{
			std::getline(file, next);
			int index = next.find_first_of("=");
			if (next.size() > 0 && next[0] != '#')
			{
				std::string key = next.substr(0, index);
				std::string content = next.substr(index + 1, next.size() - index);
				add(key, content);
			}		
		}while(!file.eof());
		file.close();
	}
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

template<typename ObjectType>
std::string Messages::Get(const char * id, ObjectType var)
{	
	std::string msg(Get(id));
	replace(msg, 0, var);
	return msg;
}


std::string Messages::get(const char * id)
{
	std::string key(id);
	if (bag.count(key) == 0)
	{
		return key;
	}
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
