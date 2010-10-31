#ifndef MACARONI_BUILD_CONFIGURATIONID_H
#define MACARONI_BUILD_CONFIGURATIONID_H

#include "../ME.h"
#include "LibraryId.h"

namespace Macaroni { namespace Build {

class ConfigurationId
{
public: 
	ConfigurationId(){}
	
	inline const LibraryId & GetLibraryId() const
	{
		return libraryId;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	void SetLibraryId(const LibraryId & value)
	{
		libraryId = value;
	}

	void SetName(const std::string & value)
	{
		name = value;
	}

private:
	LibraryId libraryId;
	std::string name;	
};


} }

#endif
