#ifndef MACARONI_COMPILEROPTIONS_H
#define MACARONI_COMPILEROPTIONS_H

#include <boost/filesystem/operations.hpp>
#include "../Gestalt/FileSystem/FileSet.h"

namespace Macaroni {

class CompilerOptions
{
public:
	CompilerOptions(const Gestalt::FileSystem::FileSet & input, 
					const boost::filesystem::path & output);
	~CompilerOptions();
	
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


}

#endif
