/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

