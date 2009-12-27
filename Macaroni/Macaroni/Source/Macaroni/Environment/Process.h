#ifndef MACARONI_ENVIRONMENT_PROCESS_H
#define MACARONI_ENVIRONMENT_PROCESS_H

#include "../ME.h"
#include "Console.h"
#include <cstdlib>
#include "Messages.h"
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Environment {

class Process
{
public:
	Process(boost::filesystem::path fileName, const std::string & args, 
		    const std::vector<const std::string> paths);
	bool Run(const Console & console);
private:
	std::string args;

	boost::filesystem::path fileName;

	/** Appends all of the path variables to the PATH environment variable. */
	void mixinPathEnvVariables();

	std::vector<const std::string> paths;
};


} }

#endif
