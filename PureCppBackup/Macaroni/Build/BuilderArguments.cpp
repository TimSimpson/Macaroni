#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_CPP

#include "BuilderArguments.h"
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

BuilderArguments::BuilderArguments(const bool  generate, const bool  compile, const bool  install)
: generate(generate), compile(compile), install(install)
{
	
    
}
//~<(Skipping inline function "Macaroni::Build::BuilderArguments::Generate")
//~<(Skipping inline function "Macaroni::Build::BuilderArguments::Compile")
//~<(Skipping inline function "Macaroni::Build::BuilderArguments::Install")

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_CPP