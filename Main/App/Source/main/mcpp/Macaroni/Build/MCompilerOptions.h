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
#ifndef MACARONI_BUILD_MCOMPILEROPTIONS_H
#define MACARONI_BUILD_MCOMPILEROPTIONS_H

#include <Macaroni/ME.h>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/IO/FileSet.h>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

/** Defines options for the Macaroni compiler.  */
class MCompilerOptions
{
public:
	//TODO: Remove the generators which goto this as an argument.
	MCompilerOptions(const std::vector<Macaroni::IO::FileSet> & input,
					 const boost::filesystem::path & output,
					 const std::vector<MACARONI_VE_CONST std::string> generators);

	~MCompilerOptions();

	inline const std::vector<MACARONI_VE_CONST std::string> GetGenerators() const
	{
		return generators;
	}

	inline const std::vector<Macaroni::IO::FileSet> & GetInput() const
	{
		return input;
	}

	inline const boost::filesystem::path & GetOutput() const
	{
		return output;
	}


private:
	const std::vector<MACARONI_VE_CONST std::string> generators;
	std::vector<Macaroni::IO::FileSet> input;
	boost::filesystem::path output;
};


} }

#endif
