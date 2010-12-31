#ifndef MACARONI_BUILD_LIBRARYID_CPP
#define MACARONI_BUILD_LIBRARYID_CPP

#include <Macaroni/ME.h>
#include "LibraryId.h"
#include <Macaroni/IO/FileNotFoundException.h>
#include <boost/format.hpp>
#include "../IO/PathResolver.h"
#include <boost/regex.hpp>
#include <sstream>

namespace Macaroni { namespace Build {

LibraryId::LibraryId(const std::string & group, 
					 const std::string & name, 
					 const std::string & version)
: group(group),
  name(name), 
  version(version)				
{
	MACARONI_CHECK(IsStringLegal(group), 
		(boost::format("Group \"%s\" invalid for library.") % group).str().c_str());
	MACARONI_CHECK(IsStringLegal(name), 
		(boost::format("Name \"%s\" invalid for library.") % name).str().c_str());
	MACARONI_CHECK(IsStringLegal(version), 
		(boost::format("Version \"%s\" invalid for library.") % version).str().c_str());
}

bool operator==(const LibraryId & a, const LibraryId & b)
{
	return a.GetGroup() == b.GetGroup()
			&& a.GetName() == b.GetName()
			&& a.GetVersion() == b.GetVersion();
}

bool LibraryId::IsStringLegal(const std::string & str)
{
	static const boost::regex legalCharacters("[A-Za-z0-9_\\-.]+");
	return boost::regex_match(str, legalCharacters);
}
	

} }

#endif
