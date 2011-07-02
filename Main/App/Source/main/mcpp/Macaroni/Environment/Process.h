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
#ifndef MACARONI_ENVIRONMENT_PROCESS_H
#define MACARONI_ENVIRONMENT_PROCESS_H

#include "../ME.h"
#include <boost/optional.hpp>
#include "Console.h"
#include <cstdlib>
#include <Macaroni/Platform/Windows/EnvironmentVariables.h>
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
		    const std::vector<MACARONI_VE_CONST std::string> paths,
			const std::vector<StringPair> & envVariables);
	bool Run(const Console & console);
private:
	std::string args;

	const std::vector<StringPair> envVariables;

	boost::optional<boost::filesystem::path> fileName;

	/** Appends all of the path variables to the PATH environment variable. */
	void mixinPathEnvVariables(Macaroni::Platform::Windows::EnvironmentVariables & vars);

	std::vector<MACARONI_VE_CONST std::string> paths;

	boost::filesystem::path workingDirectory;
};


} }

#endif
