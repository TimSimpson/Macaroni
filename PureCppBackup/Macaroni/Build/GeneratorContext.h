#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_H

// This class was originally defined in Source/Macaroni/Build/GeneratorContext.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class GeneratorContext;
} } // End namespace 

#include <Macaroni/IO/Path.h>
#include <Macaroni/Model/LibraryPtr.h>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class GeneratorContext
{
private: 	Macaroni::Model::LibraryPtr  library;
private: 	Macaroni::IO::PathPtr  path;
	public: GeneratorContext(Macaroni::Model::LibraryPtr  library, Macaroni::IO::PathPtr  path);
public: 	inline Macaroni::Model::LibraryPtr  GetLibrary()	{
		
        return library;
    
	}
public: 	inline Macaroni::IO::PathPtr &  GetPath()	{
		
        return path;
    
	}
}; // End of class GeneratorContext

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_GeneratorContext_H