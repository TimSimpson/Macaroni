#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H

// This class was originally defined in Source/Macaroni/Build/Builder.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class Builder;
} } // End namespace 

#include <Macaroni/Model/ContextPtr.h>
namespace Macaroni { namespace Build { namespace Cpp { 
class CppFile;
} } } // End namespace 
#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <boost/shared_ptr.hpp>
namespace Macaroni { namespace Build { namespace Cpp { 
class LinkerSettings;
} } } // End namespace 
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Environment/Console.h>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/Build/Configuration.h>
#include <vector>
#include <Macaroni/Build/Cpp/LinkerSettings.h>
#include <Macaroni/Build/Cpp/CppFile.h>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class Builder
{
private: 	const Macaroni::Build::Configuration &  configuration;
private: 	const Macaroni::Environment::Console &  console;
private: 	boost::shared_ptr<Macaroni::Build::Cpp::CompilerSettings >  cppCompiler;
private: 	std::vector<Macaroni::Build::Cpp::CppFile >  cppFiles;
private: 	std::vector<const std::string >  cppSrcRoots;
private: 	bool  install;
private: 	Macaroni::Model::LibraryPtr  library;
private: 	Macaroni::Build::Cpp::LinkerSettings  linkerSettings;
private: 	const Macaroni::Build::Manifest &  manifest;
	public: Builder(Macaroni::Model::ContextPtr  context, const Macaroni::Build::Manifest &  manifest, const Macaroni::Build::Configuration &  config, Macaroni::Environment::Console &  console, bool  install);
public: 	bool  CompileCpp();
public: 	bool  CompileMacaroni();
public: 	bool  CopyCppFiles(boost::filesystem::path  exportCppDir);
public: 	bool  CopyHeaderFiles(boost::filesystem::path  headersDir);
private: 	void  createCppFileList();
private: 	void  createCppSrcRoots();
private: 	void  createIncludePaths(std::vector<const std::string > &  includes);
private: 	void  createIncludePaths2(std::vector<const std::string > &  includes, const Macaroni::Build::Configuration &  config);
public: 	bool  CreateInterface();
public: 	bool  CreateInterfaceMh();
public: 	void  Execute(bool  generate, bool  saveCppSource, bool  compile, bool  install);
private: 	boost::filesystem::path  findCppCompilerSettingsFile();
public: 	bool  Install();
public: 	bool  Link();
public: 	bool  SaveCppSource();
}; // End of class Builder

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_H