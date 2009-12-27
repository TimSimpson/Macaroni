#ifndef MACARONI_BUILD_BUILDER_CPP
#define MACARONI_BUILD_BUILDER_CPP

#include "Builder.h"
#include "Cpp/CompilerSettings.h"
#include "Cpp/CppFile.h"
#include "../Gestalt/FileSystem/FileSet.h"
#include "MCompilerOptions.h"
#include "MCompiler.h"
#include "../IO/Path.h"
#include <boost/filesystem/path.hpp>
#include "../../Gestalt/FileSystem/Paths.h"

using Macaroni::Build::Cpp::CppFile;
using Macaroni::Build::Cpp::CompilerSettings;
using Macaroni::Environment::Console;
using Gestalt::FileSystem::FileSet;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;

namespace Macaroni { namespace Build {

Builder::Builder(const Manifest & manifest, const Configuration & config, Console & console)
: configuration(config),
  console(console),
  manifest(manifest)
{
}

void Builder::CompileCpp()
{
	boost::filesystem::path exePath(Gestalt::FileSystem::Paths::GetExeDirectoryPath());
	boost::filesystem::path compilerDirPath = exePath / "Compilers";
	boost::filesystem::path compilerFilePath = compilerDirPath / configuration.GetCompiler();
	if (!boost::filesystem::exists(compilerFilePath))
	{
		compilerFilePath = compilerDirPath / (configuration.GetCompiler() + ".lua");
	}

	CompilerSettings settings(compilerFilePath);
	
	std::vector<CppFile> files;
	CreateCppFileList(files);

	std::vector<CppFile>::iterator itr;
	for(itr = files.begin(); itr != files.end(); itr ++)
	{
		(*itr).Compile(settings, console);
	}
}


void Builder::CompileMacaroni()
{
	console.WriteLine("~ Compiling Macaroni Source into C++ ~");
	
	std::string mSrc = manifest.GetMSource()[0];
	std::string mOut = manifest.GetMOutput();
	FileSet input(boost::filesystem::path(mSrc), "\\.mcpp$");
	MCompilerOptions options(input, 
							 boost::filesystem::path(mOut),
							 configuration.GetGenerators());
	MCompiler compiler;
	compiler.Compile(options);	
}

void Builder::CreateCppFileList(std::vector<CppFile> & files)
{
	console.WriteLine("~ Generating listing of C++ source files. ~");
	const Path & rootDir = manifest.GetRootDirectory();
	//PathPtr mWork = rootDir.NewPath("./MWork");
	PathPtr objFilesDir = rootDir.NewPathForceSlash(manifest.GetCppOutput());
	objFilesDir->CreateDirectory();

	std::vector<const std::string> srcDirs = manifest.GetMSource();
	srcDirs.push_back(manifest.GetMOutput());

	std::vector<const std::string>::iterator itr;
	for(itr = srcDirs.begin(); itr != srcDirs.end(); itr ++)
	{
		const std::string & srcDirectory = *itr;
		boost::filesystem::path srcDirectoryPath = 
			boost::filesystem::path(srcDirectory);
		FileSet input(srcDirectoryPath, "\\.cpp$");
		FileSet::Iterator itr2 = input.Begin();
		FileSet::Iterator end = input.End();
		for(; itr2 != end; ++ itr2)
		{
			boost::filesystem::path p = *itr2;
			files.push_back(CppFile(srcDirectoryPath, 
							p, 
							objFilesDir->GetAbsolutePath()));
		}	
	}
}


void Builder::Execute()
{
	console.WriteLine("--------------------------------------------------------------------------------");
	console.Write("Configuration: "); console.WriteLine(configuration.GetName());
	console.WriteLine("--------------------------------------------------------------------------------");

	const Path & dir = manifest.GetRootDirectory();
	PathPtr mWork = dir.NewPathForceSlash("./MWork");
	mWork->CreateDirectory();

	PathPtr genSrc = mWork->NewPathForceSlash("/GeneratedSource");
	genSrc->CreateDirectory();

	CompileMacaroni();


	CompileCpp();
}

} }

#endif
