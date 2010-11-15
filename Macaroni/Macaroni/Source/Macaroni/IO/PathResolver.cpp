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

	path installRepo(Paths::GetUserPath());
	installRepo = installRepo / relativePath;
	if (boost::filesystem::exists(installRepo) && boost::filesystem::is_regular_file(installRepo))
	{
		return installRepo;
	}

	path exeNeighbor(Paths::GetExeDirectoryPath());
	exeNeighbor = exeNeighbor / relativePath;
	if (boost::filesystem::exists(exeNeighbor) && boost::filesystem::is_regular_file(exeNeighbor))
	{
		return exeNeighbor;
	}

	std::stringstream ss;
	ss << "Could not find the given relative path \"" << relativePath 
	   << "\"; not found at path \"" << local 
	   << "\", \"" << installRepo 
	   << "\", nor at \"" << exeNeighbor << "\".";
	throw FileNotFoundException(ss.str());
//	return path();
}

END_NAMESPACE2

#endif

