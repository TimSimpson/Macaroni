#ifndef MACARONI_IO_PATH_H
#define MACARONI_IO_PATH_H

#include "../ME.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

class Path;
typedef boost::shared_ptr<Path> PathPtr;
typedef std::vector<PathPtr> PathList;
typedef boost::shared_ptr<PathList> PathListPtr;

/** Directory or file relative to another directory or file.
 * This class is surprisingly useful as many Macaroni operations involve 
 * dealing with relative paths with an assumed root directory. */
class Path
{
public:
	Path(boost::filesystem::path rootPath, const char * path);
	Path(const Path & other); 
	Path(boost::filesystem::path rootPath, boost::filesystem::path path);
	
	static void CopyDirectoryContents(boost::filesystem::path & src,
									  boost::filesystem::path & dst);

	static void CopyDirectoryContents(Path & src,
									  boost::filesystem::path & dst);

	/** An example would be to copy a file from "src/blah/a.txt" to 
	 * "output/blah/a.txt", where "src" is the original root path and
	 * "output" is the argument to this function. */
	void CopyToDifferentRootPath(boost::filesystem::path rootPath);

	void CreateDirectory() const;

	bool Exists() const;

	std::string GetAbsolutePath() const;

	PathListPtr GetPaths() const;

	std::string GetRelativePath() const;

	bool IsDirectory() const;

	/** Creates a new path with the current path's name as the suffix. */
	PathPtr NewPath(const std::string & name) const;

	/** Creates a new path but forces a slash to appear. */
	PathPtr Path::NewPathForceSlash(const std::string & name) const;

	/** Opens a file in the current directory for writing. */
	// std::ofstream OpenFile(const char * name);
	// lua function OpenFile(name);

	/** The path in String form. */
	std::string ToString() const;
	
private:	

	void assertPathExistsInRootPath();

	boost::filesystem::path path;

	boost::filesystem::path rootPath;

	static bool stringBeginsWith(const std::string & str, const std::string begins);
};

END_NAMESPACE2

#endif

