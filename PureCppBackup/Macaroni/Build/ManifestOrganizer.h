#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_H

// This class was originally defined in Source/Macaroni/Build/ManifestOrganizer.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class ManifestOrganizer;
} } // End namespace 

#include <vector>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/LibraryId.h>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class ManifestOrganizer
{
private: 	std::vector<Macaroni::Build::ManifestPtr > &  list;
private: 	std::vector<std::vector<Macaroni::Build::LibraryId > >  listHistory;
	public: ManifestOrganizer(std::vector<Macaroni::Build::ManifestPtr > &  manifests);
private: 	void  clearHistory();
private: 	bool  dependencyListContains(const std::vector<Macaroni::Build::LibraryId > &  dependencies, const Macaroni::Build::LibraryId &  id);
public: 	std::vector<Macaroni::Build::ManifestPtr >  GetList();
private: 	bool  positionPreviouslyContainedId(const int  position, const Macaroni::Build::LibraryId &  id);
private: 	void  set(int  position, Macaroni::Build::ManifestPtr  manifest);
public: 	void  Sort();
private: 	bool  swap(int  a, int  b);
}; // End of class ManifestOrganizer

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_H