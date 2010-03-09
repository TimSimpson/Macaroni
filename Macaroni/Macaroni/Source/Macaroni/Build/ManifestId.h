#ifndef MACARONI_BUILD_MANIFESTID_H
#define MACARONI_BUILD_MANIFESTID_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

//TODO: Rename this to LibraryId.
/** The part of a Manifest which can identify the Module. */
class ManifestId
{
public:
	ManifestId(){}
	
	boost::filesystem::path FindFinalManifestFile() const;

	inline const std::string & GetGroup() const
	{
		return group;
	}
	
	inline const std::string & GetName() const
	{
		return name;
	}
	
	inline const std::string & GetVersion() const
	{
		return version;
	}

	
	inline void SetGroup(std::string & value)
	{
		group = value;
	}

	inline void SetName(std::string & value)
	{
		name = value;
	}

	inline void SetVersion(std::string & value)
	{
		version = value;
	}
private:
	std::string group;
	std::string name;
	std::string version;
};


} }

#endif
