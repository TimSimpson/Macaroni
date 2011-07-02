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
#ifndef MACARONI_ENVIRONMENT_CONSOLE_H
#define MACARONI_ENVIRONMENT_CONSOLE_H

#include "../ME.h"
#include "Messages.h"
#include <iostream>
#include <string>
#include <vector>

namespace Macaroni { namespace Environment {

class Console
{
public:
	Console(){}

	void Write(const char * msg) const
	{
		std::cout << msg;
	}

	void Write(const std::string & msg) const
	{
		std::cout << msg;
	}

	void WriteMessage(const char * msgKey) const
	{
		std::cout << Environment::Messages::Get(msgKey);
	}

	void WriteLine(const char * msg) const
	{
		std::cout << msg << std::endl;
	}

	void WriteLine(const wchar_t * msg) const
	{
		std::cout << msg << std::endl;
	}

	void WriteLine(const std::string & msg) const
	{
		std::cout << msg << std::endl;
	}

private:	
};


} }

#endif
