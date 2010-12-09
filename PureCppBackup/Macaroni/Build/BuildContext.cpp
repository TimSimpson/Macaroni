#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_CPP

#include "BuildContext.h"
#include <Macaroni/Model/Library.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/Path.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Build::Manifest;
using Macaroni::Build::ManifestPtr;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using boost::shared_ptr;
using std::string;
using std::vector;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

BuildContext::BuildContext(LibraryPtr  library, std::vector<Macaroni::IO::PathPtr >  sourceDirs, PathPtr  outputDir, PathPtr  installDir, const string &  properties)
: installDir(installDir), library(library), outputDir(outputDir), properties(properties), sourceDirs(sourceDirs)
{
	
    
}
//~<(Skipping inline function "Macaroni::Build::BuildContext::GetInstallDir")
//~<(Skipping inline function "Macaroni::Build::BuildContext::GetLibrary")
//~<(Skipping inline function "Macaroni::Build::BuildContext::GetOutputDir")
//~<(Skipping inline function "Macaroni::Build::BuildContext::GetProperties")
//~<(Skipping inline function "Macaroni::Build::BuildContext::GetSourceDirs")

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_CPP