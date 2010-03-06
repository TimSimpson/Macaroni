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
	Manifest();

	Manifest(const boost::filesystem::path & manifestFile);
	
	inline std::string GetDescription() const
	{
		return description;
	}	

	inline const std::string & GetCppOutput() const
	{
		return cppOutput;
	}

	inline const std::string & GetCppSourceOutput() const
	{
		return cppSourceOutput;
	}
	
	const Configuration * GetConfiguration(const std::string & configName) const;

	inline const ManifestId GetId() const 
	{
		return id;
	}

	inline const std::string GetCppHeadersOutput() const
	{
		return cppHeadersOutput;
	}

	inline const std::string GetFinalOutput() const
	{
		return fOutput;
	}

	inline const std::string & GetGroup() const
	{
		return group;
	}

	inline const boost::filesystem::path & GetManifestFile() const
	{
		return manifestFile;
	}

	inline const std::string GetMOutput() const
	{
		return mOutput;
	}

	inline const std::string & GetName() const
	{
		return name;
	}


	inline const boost::filesystem::path & GetRootDirectory() const
	{
		return rootDirectory;
	}

	inline const std::vector<const std::string> GetMSource() const
	{
		return mSource;
	}

	inline const std::string & GetVersion() const
	{
		return version;
	}

	void SaveAs(boost::filesystem::path & filePath);

	inline void SetDescription(std::string & value)
	{
		description = value;
	}

	inline void SetMSource(std::vector<const std::string> & value)
	{
		mSource = value;
	}
private:	
	std::vector<const Configuration> configurations;
	std::string cppOutput;
	std::string cppSourceOutput;
	std::string description;
	std::string fOutput;	
	//std::vector<std::string> generators;
	std::string group;
	ManifestId id;	
	std::string cppHeadersOutput;
	boost::filesystem::path manifestFile;
	std::string mOutput;
	std::vector<const std::string> mSource;
	std::string name;
	boost::filesystem::path rootDirectory;
	std::string version;
};


} }

#endif