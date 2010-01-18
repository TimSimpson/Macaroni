#ifndef MACARONI_IO_PATHRESOLVER_CPP
#define MACARONI_IO_PATHRESOLVER_CPP

#include "../ME.h"
#include "Paths.h"
#include "PathResolver.h"
#include <sstream>

using boost::filesystem::path;

BEGIN_NAMESPACE2(Macaroni, IO)

PathResolver::PathResolver()
:workingDirectory("")
{
}

PathResolver::PathResolver(path workingDirectory)
:workingDirectory(workingDirectory)
{
}

path PathResolver::FindFile(path relativePath)// throw (FileNotFoundException)
{
	path local("");
	local = local / relativePath;
	if (boost::filesystem::exists(local) && boost::filesystem::is_regular_file(local))
	{
		return local;
	}

	path user(Paths::GetUserPath());
	user = user / relativePath;
	if (boost::filesystem::exists(user) && boost::filesystem::is_regular_file(user))
	{
		return user;
	}

	path install(Paths::GetUserPath());
	install = install / relativePath;
	if (boost::filesystem::exists(install) && boost::filesystem::is_regular_file(install))
	{
		return install;
	}

	std::stringstream ss;
	ss << "Could not find the given relative path \"" << relativePath 
	   << "\"; not found at path \"" << local 
	   << "\", \"" << user 
	   << "\", nor at \"" << install << "\".";
	throw FileNotFoundException(ss.str());
//	return path();
}

END_NAMESPACE2

#endif

