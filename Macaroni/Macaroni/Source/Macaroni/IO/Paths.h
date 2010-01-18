#ifndef MACARONI_IO_PATHS_H
#define MACARONI_IO_PATHS_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

class Paths
{
public:

	/** Certain constants are stored here, such as the reference directory of
	 * Generators or libraries.  Think of this as what you'd find in the
	 * unwritable Program Files directory of Windows. */
	static std::string GetExeDirectoryPath();

	/** The path to the EXE file itself. */
	static std::string GetExePath();	

	/** The path to a directory which is writable by the user and also in a
	 * known, solid location. Example: C:/Users/SoAndSo/Macaroni. */
	static std::string GetUserPath();
};

END_NAMESPACE2

#endif

