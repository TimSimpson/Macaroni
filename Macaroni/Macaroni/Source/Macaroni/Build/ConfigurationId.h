#ifndef MACARONI_BUILD_CONFIGURATIONID_H
#define MACARONI_BUILD_CONFIGURATIONID_H

#include "../ME.h"
#include "ManifestId.h"

namespace Macaroni { namespace Build {

class ConfigurationId
{
public:
	ConfigurationId(){}
	
	inline const ManifestId & GetManifestId() const
	{
		return manifestId;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	void SetManifestId(const ManifestId & value)
	{
		manifestId = value;
	}

	void SetName(const std::string & value)
	{
		name = value;
	}

private:
	ManifestId manifestId;
	std::string name;	
};


} }

#endif
