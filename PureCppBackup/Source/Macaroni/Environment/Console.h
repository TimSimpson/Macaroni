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

	void WriteLine(const std::string & msg) const
	{
		std::cout << msg << std::endl;
	}

private:	
};


} }

#endif
