#ifndef MACARONI_BUILD_MANIFEST_H
#define MACARONI_BUILD_MANIFEST_H

#include <boost/filesystem/operations.hpp>
#include "Configuration.h"
#include "ManifestId.h"
#include "../ME.h"
#include "../IO/Path.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class Manifest
{
public:
	Manifest(const boost::filesystem::path & manifestFile);
	
	inline std::string GetDescription() const
	{
		return description;
	}

	inline const std::string & GetCppOutput() const
	{
		return cppOutput;
	}

	const Configuration * GetConfiguration(const std::string & configName) const;

	inline const ManifestId GetId() const 
	{
		return id;
	}

	inline const std::string GetMOutput() const
	{
		return mOutput;
	}

	inline const IO::Path & GetRootDirectory() const
	{
		return rootDirectory;
	}

	inline const std::vector<const std::string> GetMSource() const
	{
		return mSource;
	}

	inline void SetDescription(std::string & value)
	{
		description = value;
	}

private:	
	std::vector<const Configuration> configurations;
	std::string cppOutput;
	std::string description;
	//std::vector<std::string> generators;
	ManifestId id;	
	std::vector<const std::string> cInclude;
	std::string mOutput;
	std::vector<const std::string> mSource;
	IO::Path rootDirectory;
};


} }

#endif
