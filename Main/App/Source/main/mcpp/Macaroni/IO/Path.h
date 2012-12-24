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
#ifndef MACARONI_IO_PATH_H
#define MACARONI_IO_PATH_H

#include "../ME.h"
#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>
#include <Macaroni/IO/GeneratedFileWriter.h>
#include <Macaroni/IO/GeneratedFileWriterPtr.h>
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
	explicit Path(const std::string & absolutePath);
	explicit Path(const boost::filesystem::path & rootPath);
	Path(const boost::filesystem::path & rootPath, const char * path);
	Path(const Path & other);
	Path(const boost::filesystem::path & rootPath,
		 const boost::filesystem::path & path);

	bool operator==(const Path & other) const;

	bool operator!=(const Path & other) const;

	/** Deletes all files and directories within this directory.
	 *  Throws an exception if this is a file or doesn't exist.
	 *  Boost throws an error if the path is empty as well. */
	void ClearDirectoryContents(bool throw_if_not_exists=false);

	static void CopyDirectoryContents(boost::filesystem::path & src,
									  boost::filesystem::path & dst);

	static void CopyDirectoryContents(Path & src,
									  boost::filesystem::path & dst);

	static void CopyDirectoryContents(Path & src, Path & dst);

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

	PathPtr DifferentSuffix(const std::string & suffix);

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

	/** Grabs the filename without the extension or directory. */
	std::string GetFileNameWithoutExtension () const;

	PathPtr GetParentPath() const;

	inline boost::filesystem::path GetPath() const
	{
		return this->path;
	}

	PathListPtr GetPaths() const;

	/** Returns all paths which adhere to some regex. */
	PathListPtr GetPaths(const std::string & matchingPattern) const;

	std::string GetRelativePath() const;

	/** The relative path, but forward slashes are used even on Windows and
	 *  any leading slashes are removed (i.e. "\blah\blah.h" becomes
	 *  "blah/blah.h"). */
	std::string GetRelativePathNormalized() const;

	inline boost::filesystem::path GetRootPath() const
	{
		return this->rootPath;
	}

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

	/** Turns a string with Windows slashes to forward slashes. */
	static std::string NormalizePathString(std::string & name);

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

