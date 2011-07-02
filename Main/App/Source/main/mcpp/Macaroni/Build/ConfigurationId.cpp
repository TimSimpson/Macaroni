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
#ifndef MACARONI_BUILD_CONFIGURATIONID_CPP
#define MACARONI_BUILD_CONFIGURATIONID_CPP

#include "ConfigurationId.h"

namespace Macaroni { namespace Build {

ConfigurationId::ConfigurationId()
:libraryId(), name("")
{
}

ConfigurationId::ConfigurationId(const ConfigurationId & other)			
: libraryId(other.GetLibraryId()), name(other.GetName())
{
}

ConfigurationId & ConfigurationId::operator=(const ConfigurationId & other)
{
	this->SetLibraryId(other.GetLibraryId());
	this->SetName(other.GetName());
	return *this;
}

void ConfigurationId::SetLibraryId(const LibraryId & value)
{
	libraryId = value;
}

void ConfigurationId::SetName(const std::string & value)
{
	name = value;
}
	

} } // End NS

#endif
