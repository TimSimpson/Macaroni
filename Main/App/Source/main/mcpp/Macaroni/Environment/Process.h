#ifndef MACARONI_ENVIRONMENT_PROCESS_H
#define MACARONI_ENVIRONMENT_PROCESS_H

#include "../ME.h"
#include <boost/optional.hpp>
#include "Console.h"
#include <cstdlib>
#include "../Platform/Windows/EnvironmentVariables.h"
#include "Messages.h"
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <string>
#include "StringPair.h"
#include <vector>

namespace Macaroni { namespace Environment {

class Process
{
public:
	Process(boost::optional<boost::filesystem::path> & fileName, 
			const std::string & args, 
			boost::filesystem::path workingDirectory,
		    const std::vector<const std::string> paths,
			const std::vector<StringPair> & envVariables);
	bool Run(const Console & console);
private:
	std::string args;

	const std::vector<StringPair> envVariables;

	boost::optional<boost::filesystem::path> fileName;

	/** Appends all of the path variables to the PATH environment variable. */
	void mixinPathEnvVariables(Macaroni::Platform::Windows::EnvironmentVariables & vars);

	std::vector<const std::string> paths;

	boost::filesystem::path workingDirectory;
};


} }

#endif
