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
#ifndef MACARONI_IO_FILESET_H
#define MACARONI_IO_FILESET_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

/**
 * A collection of files.
 * It usually is constructed by iterating from a givne root directory and
 * including all files which match a pattern.
 * Note that the build environment can include patterns to exclude.
 * Also...
 * this class is... um... "inspired" by Ant's FileSet class.
 * http://ant.apache.org/manual/CoreTypes/fileset.html
 */
class FileSet
{
public:

	class Iterator
	{
	friend class FileSet;
	public:
		boost::filesystem::path operator*();

		bool operator==(const Iterator& other) const;
		
		inline bool operator!=(const Iterator& other) const
		{ 
			return !(operator==(other)); 
		}
		
		Iterator operator++();
	private:
		Iterator(const FileSet * fs);	
		bool finished();
		const FileSet * fileSet;
				
		/**
		 * Internal method to iterate through directories recursively.
		 */
		void iterate(bool freshItr);
		/**
		 * Special case- needed if this FileSet represents a single file.
		 */
		bool iterateSingleFile;

		std::vector<boost::filesystem::directory_iterator> stack;
	};

	/**
	 * Constructs a file set by iterating from the rootPath.
	 */
	FileSet(const boost::filesystem::path & rootPath);

	FileSet(const boost::filesystem::path & rootPath, boost::regex regExp);

	FileSet(const boost::filesystem::path & rootPath, const char * regExToMatch);

	FileSet(const boost::filesystem::path & rootPath, std::string regExToMatch);
	
	FileSet(const FileSet & other);

	void operator=(const FileSet & other);

	Iterator Begin() const;

	Iterator End() const;

	boost::filesystem::path GetRoot() const;
	
protected:
	/** Ensures that the path conforms to the given regular expression.
	 * Does not check to see that path is inside of root- this is used
	 * instead by the Iterator. */
	//bool testPath(boost::filesystem::path & path);
	bool testPath(const std::string & str) const;

private:
	boost::regex regExp;
	boost::filesystem::path root;
};

END_NAMESPACE2

#endif
