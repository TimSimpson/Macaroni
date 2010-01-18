#ifndef MACARONI_BUILD_MANIFESTID_CPP
#define MACARONI_BUILD_MANIFESTID_CPP

#include "ManifestId.h"
#include "../IO/PathResolver.h"

namespace Macaroni { namespace Build {

boost::filesystem::path ManifestId::FindFinalManifestFile() const
{
	boost::filesystem::path rp("Libraries");
	rp = rp / GetGroup();
	rp = rp / GetName();
	rp = rp / GetVersion();
	rp = rp / "manifest-final.lua";
	
	Macaroni::IO::PathResolver resolver;
	return resolver.FindFile(rp);
}

} }

#endif
