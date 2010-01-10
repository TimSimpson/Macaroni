#ifndef MACARONI_BUILD_NCOMPILEROPTIONS_CPP
#define MACARONI_BUILD_NCOMPILEROPTIONS_CPP

#include "MCompilerOptions.h"

using Gestalt::FileSystem::FileSet;

namespace Macaroni { namespace Build {

MCompilerOptions::MCompilerOptions(const std::vector<FileSet> & input, 
				                   const boost::filesystem::path & output,
								   const std::vector<const std::string> generators)
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
