#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_H

// This class was originally defined in Source/Macaroni/Build/BuildContext.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class BuildContext;
} } // End namespace 

#include <Macaroni/Model/LibraryPtr.h>
#include <string>
#include <vector>
#include <Macaroni/IO/Path.h>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class BuildContext
{
private: 	Macaroni::IO::PathPtr  installDir;
private: 	Macaroni::Model::LibraryPtr  library;
private: 	Macaroni::IO::PathPtr  outputDir;
private: 	std::string  properties;
private: 	std::vector<Macaroni::IO::PathPtr >  sourceDirs;
	public: BuildContext(Macaroni::Model::LibraryPtr  library, std::vector<Macaroni::IO::PathPtr >  sourceDirs, Macaroni::IO::PathPtr  outputDir, Macaroni::IO::PathPtr  installDir, const std::string &  properties);
public: 	inline Macaroni::IO::PathPtr &  GetInstallDir()	{
		
        return installDir;
    
	}
public: 	inline Macaroni::Model::LibraryPtr  GetLibrary()	{
		
        return library;
    
	}
public: 	inline Macaroni::IO::PathPtr &  GetOutputDir()	{
		
        return outputDir;
    
	}
public: 	inline const std::string &  GetProperties()	{
		
        return properties;
    
	}
public: 	inline std::vector<Macaroni::IO::PathPtr > &  GetSourceDirs()	{
		
        return sourceDirs;
    
	}
}; // End of class BuildContext

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_BuildContext_H