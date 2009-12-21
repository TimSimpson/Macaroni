#ifndef MACARONI_BUILD_NCOMPILEROPTIONS_CPP
#define MACARONI_BUILD_NCOMPILEROPTIONS_CPP

#include "MCompilerOptions.h"

using Gestalt::FileSystem::FileSet;

namespace Macaroni { namespace Build {

MCompilerOptions::MCompilerOptions(const FileSet & input, 
				                   const boost::filesystem::path & output)
:input(input),
 output(output)
{

}

MCompilerOptions::~MCompilerOptions()
{
}
	


}  } // end namespace

#endif
