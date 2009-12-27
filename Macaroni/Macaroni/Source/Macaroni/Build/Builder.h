#ifndef MACARONI_BUILD_BUILDER_H
#define MACARONI_BUILD_BUILDER_H

#include "../ME.h"
#include "../Environment/Console.h"
#include "Cpp/CppFile.h"
#include "Manifest.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class Builder
{
public:
	Builder(const Manifest & manifest, const Configuration & config, Environment::Console & console);

	void CompileCpp();

	void CreateCppFileList(std::vector<Cpp::CppFile> & files);

	void Execute();

	void CompileMacaroni();
private:
	const Configuration & configuration;
	const Environment::Console & console;
	const Manifest & manifest;
};

} }

#endif
