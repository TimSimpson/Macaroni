#ifndef MACARONI_COMPILEROPTIONS_CPP
#define MACARONI_COMPILEROPTIONS_CPP

#include "CompilerOptions.h"

using Gestalt::FileSystem::FileSet;

namespace Macaroni {

CompilerOptions::CompilerOptions(const FileSet & input, 
				                 const boost::filesystem::path & output)
:input(input),
 output(output)
{

}

CompilerOptions::~CompilerOptions()
{
}
	


}// end namespace

#endif
