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
#ifndef MACARONI_BUILD_CONFIGURATION_H
#define MACARONI_BUILD_CONFIGURATION_H

#include "../ME.h"
#include "ConfigurationId.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

/** 
 * Stores a list of dependencies and generators.
 */
class Configuration
{
public:
	Configuration(){}

	inline const std::vector<ConfigurationId> & GetDependencies() const
	{
		return dependencies;
	}

	inline const std::vector<std::string> & GetGenerators() const
	{
		return generators;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	inline void SetDependencies(const std::vector<ConfigurationId> & value)
	{
		dependencies = value;
	}

	inline void SetGenerators(const std::vector<std::string> & value)
	{
		generators = value;
	}

	inline void SetName(const std::string & value)
	{
		name = value;
	}

private:
	
	std::vector<ConfigurationId> dependencies;

	std::vector<std::string> generators;

	std::string name;
};


} }

#endif
