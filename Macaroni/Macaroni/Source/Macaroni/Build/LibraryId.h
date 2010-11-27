#ifndef MACARONI_BUILD_LIBRARYID_H
#define MACARONI_BUILD_LIBRARYID_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

//TODO: Rename this to LibraryId.
/** The part of a Manifest which can identify the Module. */
class LibraryId
{
public:
	LibraryId(){}
	
	LibraryId(const std::string & group, const std::string & name, 
			  const std::string & version)
	: group(group),
	  name(name),
	  version(version)				
	{	
	}

	friend bool operator==(const LibraryId & a, const LibraryId & b);
		
	boost::filesystem::path FindFinalManifestFile() const;

	inline const std::string & GetGroup() const
	{
		return group;
	}

	boost::filesystem::path FindInstallPath() const;
	
	inline const std::string & GetName() const
	{
		return name;
	}
	
	inline const std::string & GetVersion() const
	{
		return version;
	}

	
	inline void SetGroup(const std::string & value)
	{
		group = value;
	}

	inline void SetName(const std::string & value)
	{
		name = value;
	}

	inline void SetVersion(const std::string & value)
	{
		version = value;
	}
private:
	std::string group;
	std::string name;
	std::string version;
};

bool operator==(const LibraryId & a, const LibraryId & b);

} }

#endif
