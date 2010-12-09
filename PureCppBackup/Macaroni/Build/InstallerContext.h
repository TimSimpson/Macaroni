#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_H

// This class was originally defined in Source/Macaroni/Build/InstallerContext.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class InstallerContext;
} } // End namespace 

#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/IO/Path.h>
#include <vector>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class InstallerContext
{
private: 	Macaroni::IO::PathPtr  installDir;
private: 	Macaroni::Model::LibraryPtr  library;
private: 	Macaroni::IO::PathPtr  outputDir;
private: 	std::vector<Macaroni::IO::PathPtr >  sourceDirs;
	public: InstallerContext(Macaroni::Model::LibraryPtr  library, std::vector<Macaroni::IO::PathPtr >  sourceDirs, Macaroni::IO::PathPtr  outputDir, Macaroni::IO::PathPtr  installDir);
public: 	inline Macaroni::IO::PathPtr &  GetInstallDir()	{
		
        return installDir;
    
	}
public: 	inline Macaroni::Model::LibraryPtr  GetLibrary()	{
		
        return library;
    
	}
public: 	inline Macaroni::IO::PathPtr &  GetOutputDir()	{
		
        return outputDir;
    
	}
public: 	inline std::vector<Macaroni::IO::PathPtr > &  GetSourceDirs()	{
		
        return sourceDirs;
    
	}
}; // End of class InstallerContext

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_InstallerContext_H