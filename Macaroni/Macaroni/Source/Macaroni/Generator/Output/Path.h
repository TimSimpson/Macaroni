#ifndef MACARONI_GENERATOR_OUTPUT_DIRECTORY_H
#define MACARONI_GENERATOR_OUTPUT_DIRECTORY_H

#include "../../ME.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE(Macaroni, Generator, Output)

class Path;
typedef boost::shared_ptr<Path> PathPtr;
typedef std::vector<PathPtr> PathList;
typedef boost::shared_ptr<PathList> PathListPtr;

/** Directory or file relative to the root output folder. */
class Path
{
public:
	Path(boost::filesystem::path rootPath, const char * path);
	Path(const Path & other); 
	Path(boost::filesystem::path rootPath, boost::filesystem::path path);
	
	void CreateDirectory();

	bool Exists();

	std::string GetAbsolutePath();

	PathListPtr GetPaths();

	bool IsDirectory();

	/** Creates a new path with the current path's name as the suffix. */
	PathPtr NewPath(const std::string & name);

	/** Opens a file in the current directory for writing. */
	// std::ofstream OpenFile(const char * name);
	// lua function OpenFile(name);

	/** The path in String form. */
	std::string ToString();
	
private:	

	void assertPathExistsInRootPath();

	boost::filesystem::path path;

	boost::filesystem::path rootPath;

	static bool stringBeginsWith(const std::string & str, const std::string begins);
};

END_NAMESPACE

#endif

