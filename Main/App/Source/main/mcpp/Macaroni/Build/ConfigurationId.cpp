#ifndef MACARONI_BUILD_CONFIGURATIONID_CPP
#define MACARONI_BUILD_CONFIGURATIONID_CPP

#include "ConfigurationId.h"

namespace Macaroni { namespace Build {

ConfigurationId::ConfigurationId()
:libraryId(), name("")
{
}

ConfigurationId::ConfigurationId(const ConfigurationId & other)			
: libraryId(other.GetLibraryId()), name(other.GetName())
{
}

ConfigurationId & ConfigurationId::operator=(const ConfigurationId & other)
{
	this->SetLibraryId(other.GetLibraryId());
	this->SetName(other.GetName());
	return *this;
}

void ConfigurationId::SetLibraryId(const LibraryId & value)
{
	libraryId = value;
}

void ConfigurationId::SetName(const std::string & value)
{
	name = value;
}
	

} } // End NS

#endif
