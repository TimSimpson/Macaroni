#ifndef MACARONI_BUILD_LIBRARYID_CPP
#define MACARONI_BUILD_LIBRARYID_CPP

#include <Macaroni/ME.h>
#include "LibraryId.h"
#include <boost/foreach.hpp>
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

bool valid1stChar(const char ch)
{
	return (ch >= 'a' && ch <= 'z') 
		|| (ch >= 'A' && ch <= 'Z');
}

bool validNChar(const char ch)
{
	return (ch >= '0' && ch <= '9') || valid1stChar(ch);
}

std::string cleanUp(const std::string & original)
{
	std::stringstream str;
	int i = 0;
	BOOST_FOREACH(char ch, original)
	{
		bool isValid = (i == 0 ? valid1stChar(ch) : validNChar(ch));
		if (isValid)
		{
			str << ch;
		}
		else if (ch == '_')
		{
			std << "__";
		}
		else
		{
			int ord = (int) ch;
			str << "_" << ord << "_";
		}
		i ++;
	}
	return str.str();
}

std::string LibraryId::GetCId() const
{
	std::stringstream ss;
	ss <<  cleanUp(group) << "___"
		<< cleanUp(name);// << "___"
		//<< cleanUp(version);
	return ss.str();
}

bool LibraryId::IsStringLegal(const std::string & str)
{
	static const boost::regex legalCharacters("[A-Za-z0-9_\\-.]+");
	return boost::regex_match(str, legalCharacters);
}



} }

#endif
