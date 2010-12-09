#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_CPP

#include "GeneratorContext.h"
#include <Macaroni/Model/Library.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/Path.h>
#include <boost/shared_ptr.hpp>

using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using boost::shared_ptr;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

GeneratorContext::GeneratorContext(LibraryPtr  library, PathPtr  path)
: library(library), path(path)
{
	
    
}
//~<(Skipping inline function "Macaroni::Build::GeneratorContext::GetLibrary")
//~<(Skipping inline function "Macaroni::Build::GeneratorContext::GetPath")

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_CPP