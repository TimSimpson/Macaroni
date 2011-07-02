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
