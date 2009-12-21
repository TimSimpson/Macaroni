#ifndef MACARONI_BUILD_MCOMPILEROPTIONS_H
#define MACARONI_BUILD_MCOMPILEROPTIONS_H

#include <boost/filesystem/operations.hpp>
#include "../../Gestalt/FileSystem/FileSet.h"

namespace Macaroni { namespace Build {

/** Defines options for the Macaroni compiler.  */
class MCompilerOptions
{
public:
	MCompilerOptions(const Gestalt::FileSystem::FileSet & input, 
					const boost::filesystem::path & output);
	~MCompilerOptions();
	
	inline const Gestalt::FileSystem::FileSet & GetInput() const
	{
		return input;
	}

	inline const boost::filesystem::path & GetOutput() const
	{
		return output;
	}


private:

	Gestalt::FileSystem::FileSet input;
	boost::filesystem::path output;
};


} }

#endif
