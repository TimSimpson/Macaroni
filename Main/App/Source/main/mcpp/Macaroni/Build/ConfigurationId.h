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
#ifndef MACARONI_BUILD_CONFIGURATIONID_H
#define MACARONI_BUILD_CONFIGURATIONID_H

#include "../ME.h"
#include "LibraryId.h"

namespace Macaroni { namespace Build {

class ConfigurationId
{
public: 
	ConfigurationId();

	ConfigurationId(const ConfigurationId & other);
	
	ConfigurationId & operator=(const ConfigurationId & other);

	inline const LibraryId & GetLibraryId() const
	{
		return libraryId;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	void SetLibraryId(const LibraryId & value);

	void SetName(const std::string & value);

private:
	LibraryId libraryId;
	std::string name;	
};


} }

#endif
