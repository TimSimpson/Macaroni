#ifndef MACARONI_BUILD_LIBRARYID_H
#define MACARONI_BUILD_LIBRARYID_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

//TODO: Move this to be next to Library class.
/** The part of a Manifest which can identify the Module. */
class LibraryId
{
public:
	LibraryId(){}
	
	LibraryId(const std::string & group, 
			  const std::string & name, 
			  const std::string & version)
	: group(group),
	  name(name), 
	  version(version)				
	{	
	}

	friend bool operator==(const LibraryId & a, const LibraryId & b);
	
	/** Finds strings Turns strings with characters that are allowed in a library name,
	 * group, or version into characters that can be allowed in most OS
	 * file paths. */
	static boost::optional<std::string> FindIllegalCharacters(const std::string & str);
			
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

} } // end namespaces

#endif
