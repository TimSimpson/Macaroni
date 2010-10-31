#ifndef MACARONI_BUILD_LIBRARYID_CPP
#define MACARONI_BUILD_LIBRARYID_CPP

#include "LibraryId.h"
#include "../IO/PathResolver.h"

namespace Macaroni { namespace Build {

boost::filesystem::path LibraryId::FindFinalManifestFile() const
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
