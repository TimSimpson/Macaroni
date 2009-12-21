#ifndef MACARONI_BUILD_MANIFEST_H
#define MACARONI_BUILD_MANIFEST_H

#include <boost/filesystem/operations.hpp>
#include "ManifestId.h"
#include "../ME.h"
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
	
	/*inline const std::vector<const std::string> & GetGenerators() const 
	{
		return generators;
	}

	inline std::vector<std::string> & GetMutableGenerators()
	{
		return generators;
	}*/

	inline const ManifestId GetId() const 
	{
		return id;
	}

	inline ManifestId GetMutableId() const
	{
		return id;
	}

	inline void SetDescription(std::string & value)
	{
		description = value;
	}

private:	
	std::string description;
	//std::vector<std::string> generators;
	ManifestId id;
};


} }

#endif
