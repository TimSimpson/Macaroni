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
#ifndef MACARONI_BUILD_MCOMPILER_H
#define MACARONI_BUILD_MCOMPILER_H

#include <Macaroni/AppPaths.h>
#include "MCompilerOptions.h"
#include "../Model/ContextPtr.h"
#include <Macaroni/IO/FileSet.h>
#include "../Model/LibraryPtr.h"
#include <Macaroni/IO/RegexFileSet.h>
#include <vector>

namespace Macaroni { namespace Build {

class MCompiler
{
public:
	MCompiler(const Macaroni::AppPathsPtr & appPaths);

	bool BuildModel(Macaroni::Model::LibraryPtr library,
		const std::vector<Macaroni::IO::RegexFileSet> & inputFiles);

	void Compile(Macaroni::Model::LibraryPtr library,
		         const MCompilerOptions & options);
private:
	AppPathsPtr appPaths;
};


} }

#endif
