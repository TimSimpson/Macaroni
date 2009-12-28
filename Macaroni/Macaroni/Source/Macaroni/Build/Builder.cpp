#ifndef MACARONI_BUILD_BUILDER_CPP
#define MACARONI_BUILD_BUILDER_CPP

#include "Builder.h"
#include "Cpp/CompilerSettings.h"
#include <boost/filesystem/convenience.hpp>
#include "Cpp/CppFile.h"
#include "../Exception.h"
#include "../Gestalt/FileSystem/FileSet.h"
#include "MCompilerOptions.h"
#include "MCompiler.h"
#include "../IO/Path.h"
#include "../Parser/ParserException.h"
#include <boost/filesystem/path.hpp>
#include "../../Gestalt/FileSystem/Paths.h"
#include "../Environment/Process.h"
#include <sstream>
#include "../Model/Source.h"

using Macaroni::Build::Cpp::CppFile;
using Macaroni::Build::Cpp::CompilerSettings;
using Macaroni::Environment::Console;
using Gestalt::FileSystem::FileSet;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::Environment::Process;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

namespace Macaroni { namespace Build {

Builder::Builder(const Manifest & manifest, const Configuration & config, Console & console)
: configuration(config),
  console(console),
  cppCompiler(),
  manifest(manifest)
{
	boost::filesystem::path cppCompilerSettingsFile = findCppCompilerSettingsFile();
	cppCompiler = Cpp::CompilerSettings(cppCompilerSettingsFile);
}

bool Builder::CompileCpp()
{	
	std::vector<CppFile>::iterator itr;
	bool perfect = true;
	for(itr = cppFiles.begin(); itr != cppFiles.end(); itr ++)
	{
		CppFile & file = *itr;
		if (!file.Compile(cppCompiler, cppSrcRoots, console))
		{
			perfect = false;
		}
	}
	return perfect;
}


bool Builder::CompileMacaroni()
{
	console.WriteLine("~ Compiling Macaroni Source into C++ ~");
	
	std::string mSrc = manifest.GetMSource()[0];
	std::string mOut = manifest.GetMOutput();
	FileSet input(boost::filesystem::path(mSrc), "\\.mcpp$");
	MCompilerOptions options(input, 
							 boost::filesystem::path(mOut),
							 configuration.GetGenerators());
	MCompiler compiler;

	try
	{
		compiler.Compile(options);	
	} 
	catch(Macaroni::Exception ex)
	{
		console.WriteLine("An error occured during Macaroni phase.");
		console.WriteLine(ex.GetSource());
		console.WriteLine(ex.GetMessage());
		return false;
	}
	catch(Macaroni::Parser::ParserException pe)
	{
		console.WriteLine("Error parsing Macaroni code: ");
		console.WriteLine(pe.GetSource()->ToString());
		console.WriteLine(pe.GetMessage());
		return false;
	}
	return true;
}

void Builder::createCppSrcRoots()
{
	cppSrcRoots = manifest.GetMSource();
	cppSrcRoots.push_back(manifest.GetMOutput());
	for (int i = 0; i < cppSrcRoots.size(); i ++) 
	{
		boost::filesystem::path srcPath(cppSrcRoots[i]);
		cppSrcRoots[i] = boost::filesystem::system_complete(srcPath).string();
	}
}

void Builder::createCppFileList()
{
	cppFiles = std::vector<CppFile>();

	console.WriteLine("~ Generating listing of C++ source files. ~");
	const Path & rootDir = manifest.GetRootDirectory();
	//PathPtr mWork = rootDir.NewPath("./MWork");
	PathPtr objFilesDirRoot = rootDir.NewPathForceSlash(manifest.GetCppOutput());
	PathPtr objFilesDir = objFilesDirRoot->NewPathForceSlash(configuration.GetName());
	objFilesDir->CreateDirectory();

	std::vector<const std::string>::iterator itr;
	for(itr = cppSrcRoots.begin(); itr != cppSrcRoots.end(); itr ++)
	{
		const std::string & srcDirectory = *itr;
		boost::filesystem::path srcDirectoryPath = 
			boost::filesystem::path(srcDirectory);
		FileSet input(srcDirectoryPath, "\\.c(pp)?$");
		FileSet::Iterator itr2 = input.Begin();
		FileSet::Iterator end = input.End();
		for(; itr2 != end; ++ itr2)
		{
			boost::filesystem::path p = *itr2;
			cppFiles.push_back(CppFile(srcDirectoryPath, 
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

	bool success = false;
	if (CompileMacaroni())
	{
		createCppSrcRoots();
		createCppFileList();

		if (CompileCpp() && Link())
		{
			success = true;
		}		
	}

	if (success)
	{
		console.WriteLine(" ~ YOU WIN! ~ ");
	}
	else
	{
		console.WriteLine(" ~ GAME OVER ~ ");
	}
}

boost::filesystem::path Builder::findCppCompilerSettingsFile()
{
	boost::filesystem::path exePath(Gestalt::FileSystem::Paths::GetExeDirectoryPath());
	boost::filesystem::path compilerDirPath = exePath / "Compilers";
	boost::filesystem::path compilerFilePath = compilerDirPath / configuration.GetCompiler();
	if (!boost::filesystem::exists(compilerFilePath))
	{
		compilerFilePath = compilerDirPath / (configuration.GetCompiler() + ".lua");
	}
	return compilerFilePath;
}

bool Builder::Link()
{
	if (configuration.GetFinal().empty())
	{
		return true;
	}

	console.WriteLine(
"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LINK"
		);

	std::stringstream args;
	std::vector<CppFile>::iterator itr;
	for(itr = cppFiles.begin(); itr != cppFiles.end(); itr ++)
	{
		CppFile & file = *itr;
		args << " \"" << file.GetObjectFilePath() << "\" ";
	}
	const Path & rootDir = manifest.GetRootDirectory();
	PathPtr finalDirRoot = rootDir.NewPathForceSlash(manifest.GetFinalOutput());
	PathPtr dir = finalDirRoot->NewPathForceSlash(configuration.GetName());
	dir->CreateDirectory();
	PathPtr finalFile = dir->NewPathForceSlash(configuration.GetFinal());

	args << " /OUT:\"" << finalFile->GetAbsolutePath() << "\" ";

	args << configuration.GetAdditionalLinkerArgs();

	Process proc(cppCompiler.GetLinkerExe(),
				 args.str(),
				 dir->GetAbsolutePath(),
				 cppCompiler.GetPaths());
	proc.Run(console);
	
	return finalFile->Exists();
}

} }

#endif
