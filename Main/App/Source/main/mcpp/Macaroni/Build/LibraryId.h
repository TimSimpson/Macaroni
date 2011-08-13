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
#ifndef MACARONI_BUILD_LIBRARYID_H
#define MACARONI_BUILD_LIBRARYID_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

//TODO: Move this to be next to Library class.
/** The part of a Manifest which can identify the Module. */
class LibraryId
{
public:
	LibraryId(){}
	
	LibraryId(const std::string & group, 
			  const std::string & name, 
			  const std::string & version);

	friend bool operator==(const LibraryId & a, const LibraryId & b);
	
	/** Returns a version of this ID which can be used as a C identifier. 
	 * The point isn't to make a pretty name but just one that maps uniquely. 
	 */
	std::string GetCId() const;

	/** Determines whether or not a string can be used for a group, name,
	 *  or version string of a library. */
	static bool IsStringLegal(const std::string & str);
			
	inline const std::string & GetGroup() const
	{
		return group;
	}	
	
	inline const std::string & GetName() const
	{
		return name;
	}
	
	inline const std::string & GetVersion() const
	{
		return version;
	}

	
	inline void SetGroup(const std::string & value)
	{
		group = value;
	}

	inline void SetName(const std::string & value)
	{
		name = value;
	}

	inline void SetVersion(const std::string & value)
	{
		version = value;
	}
private:
	std::string group;
	std::string name;
	std::string version;
};

bool operator==(const LibraryId & a, const LibraryId & b);

} } // end namespaces

#endif