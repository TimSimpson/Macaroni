#ifndef MACARONI_BUILD_LIBRARYID_CPP
#define MACARONI_BUILD_LIBRARYID_CPP

#include "LibraryId.h"
#include "../IO/PathResolver.h"

namespace Macaroni { namespace Build {

bool operator==(const LibraryId & a, const LibraryId & b)
{
	return a.GetGroup() == b.GetGroup()
			&& a.GetName() == b.GetGroup()
			&& a.GetVersion() == b.GetVersion();
}

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

boost::filesystem::path LibraryId::FindInstallPath() const
{
	boost::filesystem::path file = FindFinalManifestFile();
	return file.branch_path();
}

} }

#endif
