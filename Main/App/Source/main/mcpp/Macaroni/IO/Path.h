#ifndef MACARONI_IO_PATH_H
#define MACARONI_IO_PATH_H

#include "../ME.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <Macaroni/IO/GeneratedFileWriter.h>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

class Path;
typedef boost::shared_ptr<Path> PathPtr;
typedef std::vector<PathPtr> PathList;
typedef boost::shared_ptr<PathList> PathListPtr;

/** Directory or file path.
 * Strangely the path you are manipulating is relative to another directory or
 * file path- for example, Path("C:\Progs") defines the path C:\Progs relative
 * to itself, but Path("C:\Progs", "C:\Progs\Main") defines the path "Main"
 * relative to C:\Progs.  The motive is that many operations involve either
 * iterating directories or generating files from some well known starting
 * point.  An example of this is C++ includes with ankle brackets, which specify
 * relative paths.  Macaroni needs to work with files this way, which is why
 * I made this class a bit more complicated than might seem needed at first
 * glance. */
//TODO: Rename "RelativePath"
class Path
{
public:
	Path(const std::string & absolutePath);
	Path(boost::filesystem::path rootPath);
	Path(boost::filesystem::path rootPath, const char * path);
	Path(const Path & other); 
	Path(boost::filesystem::path rootPath, boost::filesystem::path path);
	
	/** Deletes all files and directories within this directory. 
	 *  Throws an exception if this is a file or doesn't exist.
	 *  Boost throws an error if the path is empty as well. */
	void ClearDirectoryContents();

	static void CopyDirectoryContents(boost::filesystem::path & src,
									  boost::filesystem::path & dst);

	static void CopyDirectoryContents(Path & src,
									  boost::filesystem::path & dst);

	/** An example would be to copy a file from "src/blah/a.txt" to 
	 * "output/blah/a.txt", where "src" is the original root path and
	 * "output" is the argument to this function. */
	void CopyToDifferentRootPath(boost::filesystem::path rootPath,
								 bool overrideIfExist = false);
	
	void CopyToDifferentRootPath(const PathPtr & rootPath, bool overrideIfExist = false);
		
	/** Creates the directory defined by this path, if it doesn't exist. */
	void CreateDirectory() const;

	/** Opens a file in the current directory for writing. */
	GeneratedFileWriterPtr CreateFile() const;		

	/** Creates a new instance with the current path as the absolute path. */
	PathPtr CreateWithCurrentAsRoot() const;

	/** Creates a new instance with the given path as the root path. */
	PathPtr CreateWithDifferentRootPath(const PathPtr & path);
	
	bool Exists() const;

	std::string GetAbsolutePath() const;

	/** This name sucks.  What it means is that forward slashes are used even
	 * on Windows. 
	 * TODO: Change name as it sucks, as stated above.
	 */
	std::string GetAbsolutePathForceSlash() const;

	/** Grabs the filename, i.e. everything after the directory. 
	 *  Throws if this is not a regular file. */
	std::string GetFileName() const;	
	
	PathPtr GetParentPath() const;

	PathListPtr GetPaths() const;

	/** Returns all paths which adhere to some regex. */
	PathListPtr GetPaths(const std::string & matchingPattern) const;

	std::string GetRelativePath() const;

	/** Creates a string to get us from the relative path back to root. */
	std::string GetStringBackToRoot() const;

	/** Returns a PathPtr whose root and path are pointing to this
	 *  Paths root. */
	PathPtr GetRoot() const;

	bool IsDirectory() const;

	bool IsRegularFile() const;

	/** Compares this path's file to another one. */
	bool IsFileOlderThan(const std::string & filePath) const;

	/** Creates a new path with the current path's name as the suffix. */
	PathPtr NewPath(const std::string & name) const;

	/** Creates a new path but forces a slash to appear. */
	PathPtr NewPathForceSlash(const std::string & name) const;

	/** Renames the file using only the relative part of the file path (in 
	    other words, the root path stays the same). */
	void RenameRelative(const std::string & relativePath);

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

