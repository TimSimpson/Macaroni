#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_H

// This class was originally defined in Source/Macaroni/Build/BuilderArguments.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class BuilderArguments;
} } // End namespace 


namespace Macaroni { namespace Build { 


/* Public Global Methods */

class BuilderArguments
{
private: 	bool  generate;
private: 	bool  compile;
private: 	bool  install;
	public: BuilderArguments(const bool  generate, const bool  compile, const bool  install);
public: 	inline bool  Generate() const	{
		
        return generate || Compile();
    
	}
public: 	inline bool  Compile() const	{
		
        return compile || Install();
    
	}
public: 	inline bool  Install() const	{
		
        return install;
    
	}
}; // End of class BuilderArguments

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_BuilderArguments_H