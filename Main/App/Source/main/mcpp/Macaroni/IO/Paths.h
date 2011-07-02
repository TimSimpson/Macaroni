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
#ifndef MACARONI_IO_PATHS_H
#define MACARONI_IO_PATHS_H

#include "../ME.h"
#include <boost/filesystem/operations.hpp>
#include <boost/regex.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, IO)

class Paths
{
public:

	/** Certain constants are stored here, such as the reference directory of
	 * Generators or libraries.  Think of this as what you'd find in the
	 * unwritable Program Files directory of Windows. */
	static std::string GetExeDirectoryPath();

	/** The path to the EXE file itself. */
	static std::string GetExePath();	

	/** The path to a directory which is writable by the user and also in a
	 * known, solid location. Example: C:/Users/SoAndSo/Macaroni. */
	static std::string GetUserPath();
};

END_NAMESPACE2

#endif

