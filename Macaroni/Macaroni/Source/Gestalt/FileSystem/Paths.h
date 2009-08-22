#ifndef GESTALT_FILESYSTEM_PATHS_H
#define GESTALT_FILESYSTEM_PATHS_H

#include "../../Macaroni/ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <vector>

BEGIN_NAMESPACE2(Gestalt, FileSystem)

class Paths
{
public:
	static std::string GetExeDirectoryPath();

	static std::string GetExePath();	
};

END_NAMESPACE2

#endif

