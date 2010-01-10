#ifndef MACARONI_BUILD_BUILDER_H
#define MACARONI_BUILD_BUILDER_H

#include "../ME.h"
#include "../Environment/Console.h"
#include "Cpp/CompilerSettings.h"
#include "../Model/Context.lh"
#include "Cpp/CppFile.h"
#include "../Model/Library.lh"
#include "Manifest.h"
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class Builder
{
public:
	Builder(Macaroni::Model::ContextPtr context, 
		    const Manifest & manifest, 
			const Configuration & config, 
			Environment::Console & console);

	bool CompileCpp();	

	bool CompileMacaroni();

	bool CopyHeaderFiles();

	bool CreateInterface();

	bool CreateInterfaceMh();

	void Execute();

	bool Link();

private:
	Cpp::CompilerSettings cppCompiler;
	std::vector<Cpp::CppFile> cppFiles;
	std::vector<const std::string> cppSrcRoots;
	const Configuration & configuration;
	const Environment::Console & console;
	boost::filesystem::path findCppCompilerSettingsFile();
	Macaroni::Model::LibraryPtr library;
	const Manifest & manifest;		
	
	void createCppFileList();

	void createCppSrcRoots();
};

} }

#endif
