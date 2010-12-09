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

	inline const std::vector<const ConfigurationId> & GetDependencies() const
	{
		return dependencies;
	}

	inline const std::vector<const std::string> & GetGenerators() const
	{
		return generators;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	inline void SetDependencies(std::vector<const ConfigurationId> & value)
	{
		dependencies = value;
	}

	inline void SetGenerators(std::vector<const std::string> & value)
	{
		generators = value;
	}

	inline void SetName(const std::string & value)
	{
		name = value;
	}

private:
	
	std::vector<const ConfigurationId> dependencies;

	std::vector<const std::string> generators;

	std::string name;
};


} }

#endif
