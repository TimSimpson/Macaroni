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
#ifndef MACARONI_BUILD_NCOMPILEROPTIONS_CPP
#define MACARONI_BUILD_NCOMPILEROPTIONS_CPP

#include "MCompilerOptions.h"

using Macaroni::IO::FileSet;

namespace Macaroni { namespace Build {

MCompilerOptions::MCompilerOptions(const std::vector<FileSet> & input, 
				                   const boost::filesystem::path & output,
								   const std::vector<MACARONI_VE_CONST std::string> generators)
:input(input),
 output(output),
 generators(generators)
{

}

MCompilerOptions::~MCompilerOptions()
{
}
	


}  } // end namespace

#endif
