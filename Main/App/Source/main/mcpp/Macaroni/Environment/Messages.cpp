/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_ENVIRONMENT_MESSAGES_CPP
#define MACARONI_ENVIRONMENT_MESSAGES_CPP

#include "Messages.h"

#include <fstream>
#include <boost/format.hpp>
#include <Macaroni/IO/Paths.h>
#include <sstream>
#include <string>

using boost::format;
using Macaroni::IO::Paths;
using std::string;

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
	string finalFilePath;
	if (filePath != nullptr)
	{
		finalFilePath = filePath;
	}
	else
	{

		finalFilePath = str(format("%s%s%s") % Paths::GetExeDirectoryPath()
		                    % MACARONI_DIRECTORY_SEPERATOR % "Messages.txt");
	}
	std::ifstream file;
	file.open(finalFilePath.c_str());
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
	else
	{
		string msg = str(format("Messages file not not found at \"%s\".")
		                 % finalFilePath);
		MACARONI_THROW(msg.c_str());
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

Messages & Messages::getInstance(const char * filePath)
{
	static Messages * instance = nullptr;
	if (instance == nullptr)
	{
		instance = new Messages(filePath);
	}
	return dynamic_cast<Messages &>(*instance);
}

void Messages::Init(const char * filePath)
{
	getInstance(filePath);
}


END_NAMESPACE2

#endif
