#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H

// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class Builder;
} } // End namespace 

#include <Macaroni/Model/Library.lh>
namespace Macaroni { namespace Build { namespace Cpp { 
class LinkerSettings;
} } } // End namespace 
#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <boost/shared_ptr.hpp>
namespace Macaroni { namespace Build { namespace Cpp { 
class CppFile;
} } } // End namespace 
#include <Macaroni/Model/Context.lh>
#include <Macaroni/Environment/Console.h>
#include <Macaroni/Build/Configuration.h>
#include <string>
#include <Macaroni/Build/Manifest.h>
#include <vector>
#include <Macaroni/Build/Cpp/LinkerSettings.h>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/Build/Cpp/CppFile.h>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class Builder
{
public:
	Builder(Macaroni::Model::ContextPtr  context, const Macaroni::Build::Manifest &  manifest, const Macaroni::Build::Configuration &  config, Macaroni::Environment::Console &  console, bool  install);
	bool  CompileCpp();
	bool  CompileMacaroni();
	bool  CopyHeaderFiles(boost::filesystem::path  headersDir);
	bool  CreateInterface();
	bool  CreateInterfaceMh();
	void  Execute(bool  generate, bool  compile, bool  install);
	bool  Install();
	bool  Link();
protected:
private:
	const Macaroni::Build::Configuration &  configuration;
	const Macaroni::Environment::Console &  console;
	boost::shared_ptr<Macaroni::Build::Cpp::CompilerSettings >  cppCompiler;
	std::vector<Macaroni::Build::Cpp::CppFile >  cppFiles;
	std::vector<const std::string >  cppSrcRoots;
	bool  install;
	Macaroni::Model::LibraryPtr  library;
	Macaroni::Build::Cpp::LinkerSettings  linkerSettings;
	const Macaroni::Build::Manifest &  manifest;
	void  createCppFileList();
	void  createCppSrcRoots();
	void  createIncludePaths(std::vector<const std::string > &  includes);
	void  createIncludePaths2(std::vector<const std::string > &  includes, const Macaroni::Build::Configuration &  config);
	boost::filesystem::path  findCppCompilerSettingsFile();
}; // End of class Builder

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H