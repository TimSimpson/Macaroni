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
#ifndef MACARONI_PLATFORM_WINDOWS_FILETIME_H
#define MACARONI_PLATFORM_WINDOWS_FILETIME_H

#include "../../ME.h"
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Platform, Windows)

class FileTime
{
public:
	static bool File1IsOlderThanFile2(boost::filesystem::path & one,
									  boost::filesystem::path & two);
};

END_NAMESPACE

#endif
