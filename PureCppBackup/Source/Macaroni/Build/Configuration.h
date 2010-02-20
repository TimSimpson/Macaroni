#ifndef MACARONI_BUILD_CONFIGURATION_H
#define MACARONI_BUILD_CONFIGURATION_H

#include "../ME.h"
#include "ConfigurationId.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

/** 
 * Stores a list of dependencies and generators, as well as the C++ compiler.
 */
class Configuration
{
public:
	Configuration(){}

	inline const std::string GetAdditionalLinkerArgs() const
	{
		return additionalLinkerArgs;
	}

	inline const std::vector<const std::string> & GetCInclude() const
	{
		return cInclude;
	}

	inline std::vector<const std::string> & GetMutableCInclude()
	{
		return cInclude;
	}

	inline const std::string GetCompiler() const
	{
		return compiler;
	}

	inline const std::vector<const ConfigurationId> & GetDependencies() const
	{
		return dependencies;
	}

	inline const std::string GetFinal() const
	{
		return final;
	}

	inline const std::vector<const std::string> & GetGenerators() const
	{
		return generators;
	}

	inline const std::vector<const std::string> & GetLinkerLibraries() const
	{
		return linkerLibraries;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	inline void SetAdditionalLinkerArgs(std::string & value)
	{
		additionalLinkerArgs = value;
	}

	inline void SetCInclude(std::vector<const std::string> & value)
	{
		cInclude = value;
	}

	inline void SetCompiler(std::string & value)
	{
		compiler = value;
	}

	inline void SetDependencies(std::vector<const ConfigurationId> & value)
	{
		dependencies = value;
	}

	inline void SetFinal(const std::string & value)
	{
		final = value;
	}

	inline void SetGenerators(std::vector<const std::string> & value)
	{
		generators = value;
	}

	inline void SetLinkerLibraries(std::vector<const std::string> & value)
	{
		linkerLibraries = value;
	}

	inline void SetName(const std::string & value)
	{
		name = value;
	}

private:
	std::string additionalLinkerArgs;

	std::vector<const std::string> cInclude;
	
	std::string compiler;

	std::vector<const ConfigurationId> dependencies;

	std::string final;

	std::vector<const std::string> generators;

	std::vector<const std::string> linkerLibraries;

	std::string name;
};


} }

#endif
