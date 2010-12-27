#ifndef MACARONI_BUILD_LIBRARYID_CPP
#define MACARONI_BUILD_LIBRARYID_CPP

#include "LibraryId.h"
#include <Macaroni/IO/FileNotFoundException.h>
#include "../IO/PathResolver.h"
#include <sstream>

namespace Macaroni { namespace Build {

bool operator==(const LibraryId & a, const LibraryId & b)
{
	return a.GetGroup() == b.GetGroup()
			&& a.GetName() == b.GetName()
			&& a.GetVersion() == b.GetVersion();
}

std::string LibraryId::CreateFilePathSafeString(const std::string & str)
{
	std::stringstream ss;
	BOOST_FOREACH(const char c, str)
	{
		ss << c;
	}		
	return ss.str();
}
	

} }

#endif
