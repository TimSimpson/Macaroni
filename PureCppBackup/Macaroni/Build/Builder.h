#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H

// This class was originally defined in Source/Macaroni/Build/Builder.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class Builder;
} } // End namespace 

#include <Macaroni/Build/MCompiler.h>
namespace Macaroni { namespace Build { namespace Cpp { 
class CppFile;
} } } // End namespace 
#include <Macaroni/Environment/Console.h>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/Model/ContextPtr.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <string>
#include <Macaroni/Build/LibraryId.h>
#include <vector>
#include <Macaroni/Build/Cpp/CppFile.h>
#include <Macaroni/Build/Manifest.h>
namespace Macaroni { namespace Build { 
class BuilderArguments;
} } // End namespace 
namespace Macaroni { namespace Build { 
class BuilderArguments;
} } // End namespace 
#include <Macaroni/Build/BuilderArguments.h>
namespace Macaroni { namespace Build { 
class BuilderArguments;
} } // End namespace 

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class Builder
{
private: 	const Macaroni::Environment::Console &  console;
private: 	Macaroni::Model::ContextPtr  context;
private: 	std::vector<Macaroni::Build::Cpp::CppFile >  cppFiles;
private: 	std::vector<const std::string >  cppSrcRoots;
private: 	bool  initialized;
private: 	Macaroni::Model::LibraryPtr  library;
private: 	Macaroni::Build::ManifestPtr  manifest;
private: 	boost::filesystem::path  manifestPath;
private: 	Macaroni::Build::MCompiler  compiler;
private: 	std::string  properties;
	public: Builder(Macaroni::Model::ContextPtr  context, boost::filesystem::path  manifestPath, Macaroni::Environment::Console &  console);
private: 	boost::filesystem::path  climbToTop(std::vector<boost::filesystem::path > &  manifests, boost::filesystem::path  manifestPath);

    private :  BuildContextPtr createBuildContext(ManifestPtr manifest, Macaroni::Model::LibraryPtr library);
    private: 	Macaroni::Model::LibraryPtr  createLibraryForManifest(Macaroni::Build::ManifestPtr  manifest);
public: 	bool  Execute(const Macaroni::Build::BuilderArguments &  arguments);
private: 	bool  execute(const Macaroni::Build::BuilderArguments &  arguments, const Macaroni::Build::ManifestPtr  manifest, const Macaroni::Model::LibraryPtr  library);
private: 	boost::filesystem::path  getInstallPath(Macaroni::Build::ManifestPtr  manifest);
public: 	bool  Init();
public: 	bool  Install(Macaroni::Build::ManifestPtr  manifest, Macaroni::Model::LibraryPtr  library);
private: 	void  loadManifest(boost::filesystem::path  manifestPath, Macaroni::Build::ManifestPtr &  outManifest, Macaroni::Model::LibraryPtr &  outLibrary);
public: 	bool  parseChildren(const Macaroni::Build::BuilderArguments &  arguments, const Macaroni::Build::ManifestPtr  manifest, const Macaroni::Model::LibraryPtr  library);
private: 	void  parseDependencies(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, Macaroni::Build::ManifestPtr  sourceManifest, Macaroni::Model::LibraryPtr  sourceLibrary);
private: 	void  parseDependency(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, Macaroni::Build::ManifestPtr  sourceManifest, Macaroni::Model::LibraryPtr  sourceLibrary, const Macaroni::Build::LibraryId  dependencyId);
public: 	bool  ParseMacaroniSource(Macaroni::Build::ManifestPtr  manifest, Macaroni::Model::LibraryPtr  library);
public: 	bool  parseManifest(Macaroni::Build::ManifestPtr &  manifest, Macaroni::Model::LibraryPtr &  library);
private: 	void  parseManifestSource(Macaroni::Build::ManifestPtr  sourceManifest, Macaroni::Model::LibraryPtr  sourceLibrary);
private: 	void  parseNestedLibraries(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, Macaroni::Build::ManifestPtr  sourceManifest, Macaroni::Model::LibraryPtr  sourceLibrary);
private: 	void  printOutDependencyStack(std::vector<Macaroni::Build::LibraryId > &  dependencyStack);
public: 	std::string  RunInitialScript();
}; // End of class Builder

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H