#ifndef MACARONI_BUILD_CONFIGURATION_H
#define MACARONI_BUILD_CONFIGURATION_H

#include "../ME.h"
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

	inline const std::string GetCompiler() const
	{
		return compiler;
	}

	inline const std::vector<const std::string> & GetGenerators() const
	{
		return generators;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	inline void SetCompiler(std::string & value)
	{
		compiler = value;
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
	std::string compiler;

	std::vector<const std::string> generators;

	std::string name;
};


} }

#endif
