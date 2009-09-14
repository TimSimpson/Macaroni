#ifndef MACARONI_GENERATOR_OUTPUT_DIRECTORY_H
#define MACARONI_GENERATOR_OUTPUT_DIRECTORY_H

#include "../../ME.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE(Macaroni, Generator, Output)

class Path;
typedef std::vector<Path> PathList;
typedef boost::shared_ptr<Path> PathPtr;
typedef boost::shared_ptr<PathList> PathListPtr;

/** Directory or file relative to the root output folder. */
class Path
{
public:
	Path(const char * path);
	Path(const Path & other); 
	Path(boost::filesystem::path path);

	PathListPtr GetPaths();
	
	/** Opens a file in the current directory for writing. */
	// std::ofstream OpenFile(const char * name);
	// lua function OpenFile(name);

	/** The path in String form. */
	std::string ToString();
	
private:	

	boost::filesystem::path path;
};

END_NAMESPACE

#endif

