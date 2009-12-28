#ifndef MACARONI_BUILD_BUILDER_H
#define MACARONI_BUILD_BUILDER_H

#include "../ME.h"
#include "../Environment/Console.h"
#include "Cpp/CompilerSettings.h"
#include "Cpp/CppFile.h"
#include "Manifest.h"
#include <boost/filesystem/path.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class Builder
{
public:
	Builder(const Manifest & manifest, const Configuration & config, Environment::Console & console);

	bool CompileCpp();	

	bool CompileMacaroni();

	void Execute();

	bool Link();

private:
	Cpp::CompilerSettings cppCompiler;
	std::vector<Cpp::CppFile> cppFiles;
	std::vector<const std::string> cppSrcRoots;
	const Configuration & configuration;
	const Environment::Console & console;
	boost::filesystem::path findCppCompilerSettingsFile();
	const Manifest & manifest;	

	void createCppFileList();

	void createCppSrcRoots();
};

} }

#endif
