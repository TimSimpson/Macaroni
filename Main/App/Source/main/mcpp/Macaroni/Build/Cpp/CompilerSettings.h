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
#ifndef MACARONI_BUILD_CPP_COMPILERSETTINGS_H
#define MACARONI_BUILD_CPP_COMPILERSETTINGS_H

#include <Macaroni/Environment/Console.h>
#include <boost/filesystem/operations.hpp>
#include <string>
#include "../../Environment/StringPair.h"
#include <vector>

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

class CompilerSettings
{
public:
	CompilerSettings();

	CompilerSettings(const boost::filesystem::path & filePath);

	inline const std::string & GetAdditionalCompilerArgs() const
	{
		return additionalCompilerArgs;
	}

	inline const std::string & GetAdditionalLinkerArgs() const
	{
		return additionalLinkerArgs;
	}

	inline const std::string & GetCompilerExe() const
	{
		return compilerExe;
	}

	inline const std::vector<Macaroni::Environment::StringPair> & GetEnvironmentVariables() const
	{
		return environmentVariables;
	}

	inline const std::vector<MACARONI_VE_CONST std::string> & GetIncludePaths() const
	{
		return includePaths;
	}

	inline const std::string & GetLinkerExe() const
	{
		return linkerExe;
	}

	inline const std::vector<MACARONI_VE_CONST std::string> & GetLinkerLibraryPaths() const
	{
		return linkerLibraryPaths;
	}

	inline const std::vector<MACARONI_VE_CONST std::string> & GetPaths() const
	{
		return paths;
	}

	inline const std::vector<Macaroni::Environment::StringPair> & GetPreprocessorDirectives() const
	{
		return preprocessorDirectives;
	}

	inline void SetIncludePaths(std::vector<MACARONI_VE_CONST std::string> & value)
	{
		includePaths = value;
	}

private:
	typedef std::vector<MACARONI_VE_CONST std::string> StrVec;

	std::string additionalCompilerArgs;
	std::string additionalLinkerArgs;
	std::string compilerExe;
	std::vector<Macaroni::Environment::StringPair> environmentVariables;
	std::string firstLinkedObjects;
	StrVec includePaths;
	std::string linkerExe;
	StrVec linkerLibraryPaths;
	std::string oSwitch;
	StrVec paths;
	std::vector<Macaroni::Environment::StringPair> preprocessorDirectives;
};


END_NAMESPACE

#endif
