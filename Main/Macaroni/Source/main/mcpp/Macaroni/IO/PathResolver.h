#ifndef MACARONI_IO_PATHRESOLVER_H
#define MACARONI_IO_PATHRESOLVER_H

#include "../ME.h"
#include "FileNotFoundException.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

class PathResolver;

/** Utility class which helps look for files which may be in the following
 * locations:
 *  * Local directory (current working directory).
 *  * User directory.
 *  * Exe's directory.
 **/
class PathResolver
{
public:
	/** Finds the current working directory. */
	PathResolver();
	
	PathResolver(boost::filesystem::path workingDirectory);

	boost::filesystem::path FindFile(boost::filesystem::path relativePath);// throw (FileNotFoundException);
private:
	boost::filesystem::path workingDirectory;
};

END_NAMESPACE2

#endif

