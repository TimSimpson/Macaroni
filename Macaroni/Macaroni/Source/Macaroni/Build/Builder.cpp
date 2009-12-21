#ifndef MACARONI_BUILD_BUILDER_CPP
#define MACARONI_BUILD_BUILDER_CPP

#include "Builder.h"
#include "../Gestalt/FileSystem/FileSet.h"
#include "MCompilerOptions.h"
#include "MCompiler.h"

using Macaroni::Environment::Console;
using Gestalt::FileSystem::FileSet;

namespace Macaroni { namespace Build {

Builder::Builder(const Manifest & manifest, const Configuration & config, Console & console)
: configuration(config),
  console(console),
  manifest(manifest)
{
}

void Builder::CompileCpp()
{
	console.Write("~ Compiling C++ into Objects ~");

}

void Builder::CompileMacaroni()
{
	console.Write("~ Compiling Macaroni Source into C++ ~");
	
	std::string mSrc = manifest.GetMSource()[0];
	std::string mOut = manifest.GetMOutput();
	FileSet input(boost::filesystem::path(mSrc), "\\.mcpp$");
	MCompilerOptions options(input, 
							 boost::filesystem::path(mOut),
							 configuration.GetGenerators());
	MCompiler compiler;
	compiler.Compile(options);	
}

void Builder::Execute()
{
	console.WriteLine("--------------------------------------------------------------------------------");
	console.Write("Configuration: "); console.WriteLine(configuration.GetName());
	console.WriteLine("--------------------------------------------------------------------------------");

	CompileMacaroni();

	CompileCpp();
}

} }

#endif
