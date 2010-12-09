#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_CPP

#include "InstallerContext.h"
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

InstallerContext::InstallerContext(LibraryPtr  library, std::vector<Macaroni::IO::PathPtr >  sourceDirs, PathPtr  outputDir, PathPtr  installDir)
: installDir(installDir), library(library), outputDir(outputDir), sourceDirs(sourceDirs)
{
	
    
}
//~<(Skipping inline function "Macaroni::Build::InstallerContext::GetInstallDir")
//~<(Skipping inline function "Macaroni::Build::InstallerContext::GetLibrary")
//~<(Skipping inline function "Macaroni::Build::InstallerContext::GetOutputDir")
//~<(Skipping inline function "Macaroni::Build::InstallerContext::GetSourceDirs")

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_CPP