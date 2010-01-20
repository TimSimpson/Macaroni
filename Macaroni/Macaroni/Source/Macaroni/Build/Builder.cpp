#ifndef MACARONI_BUILD_BUILDER_CPP
#define MACARONI_BUILD_BUILDER_CPP

#include "Builder.h"
#include "Cpp/CompilerSettings.h"
#include "../Model/Context.h"
#include <boost/filesystem/convenience.hpp>
#include "Cpp/CppFile.h"
#include "../Exception.h"
#include "../IO/FileSet.h"
#include "../Model/Library.h"
#include "MCompilerOptions.h"
#include "MCompiler.h"
#include "../IO/Path.h"
#include "../Parser/ParserException.h"
#include <boost/filesystem/path.hpp>
#include "../IO/Paths.h"
#include "../Environment/Process.h"
#include <sstream>
#include "../Model/Source.h"

using Macaroni::Build::Cpp::CppFile;
using Macaroni::Build::Cpp::CompilerSettings;
using Macaroni::Environment::Console;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::IO::FileSet;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using boost::filesystem::path;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::IO::Paths;
using Macaroni::Environment::Process;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;

namespace Macaroni { namespace Build {

//TODO: Library dependencies are not added recursively; i.e. all of the dependencies
//libs are added but nothing past that.
// TODO: ditto with include files.
Builder::Builder(ContextPtr context, const Manifest & manifest, const Configuration & config, Console & console, bool install)
: configuration(config),
  console(console),  
  cppCompiler(),
  install(install),
  library(context->CreateLibrary(manifest.GetName(), manifest.GetVersion())),
  manifest(manifest)
{
	if (configuration.GetCompiler() != "skip")
	{		
		boost::filesystem::path cppCompilerSettingsFile = findCppCompilerSettingsFile();
		cppCompiler = boost::shared_ptr<CompilerSettings>(new CompilerSettings(cppCompilerSettingsFile));
	}
}

bool Builder::CompileCpp()
{	
	if (!cppCompiler)
	{
		std::stringstream ss;
		ss << "CompileCpp called but no compiler is set for this configuration "
			<< "(configuration " << configuration.GetName() << " for "
			<< manifest.GetGroup() << "/" << manifest.GetName() << " version "
			<< manifest.GetVersion() << ").";
		throw Macaroni::Exception(ss.str().c_str());
	}

	// Add include paths of the dependencies
	std::vector<const std::string> includes;
	createIncludePaths(includes);
	cppCompiler->SetIncludePaths(includes);
	// -
	
	std::vector<CppFile>::iterator itr;
	bool perfect = true;
	for(itr = cppFiles.begin(); itr != cppFiles.end(); itr ++)
	{
		CppFile & file = *itr;
		if (!file.Compile(*(cppCompiler.get()), cppSrcRoots, console))
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
	std::vector<FileSet> inputFiles;
	
	MCompiler compiler;

	// Include dependencies
	const std::vector<const ConfigurationId> & dependencies = this->configuration.GetDependencies();
	for (unsigned int i = 0; i < dependencies.size(); i ++)
	{
		const ManifestId & id = dependencies[i].GetManifestId();
		Manifest dManifest(id.FindFinalManifestFile());
		const std::vector<const std::string> dSrc = dManifest.GetMSource();
		for (unsigned int j = 0; j < dSrc.size(); j ++)
		{
			LibraryPtr dLib = library->GetContext()->CreateLibrary(
				dManifest.GetName(), dManifest.GetVersion());			
			std::vector<FileSet> dInput;
			dInput.push_back(FileSet(boost::filesystem::path(dSrc[j]), "\\.m(cpp|h)?$"));
			compiler.BuildModel(dLib, dInput);			
			//compiler.BuildModel(library, dInput);
		}
	}
	// end include dependencies

	inputFiles.push_back(FileSet(boost::filesystem::path(mSrc), "\\.mcpp$"));

	MCompilerOptions options(inputFiles, 
							 boost::filesystem::path(mOut),
							 configuration.GetGenerators());	

	try
	{
		compiler.Compile(library, options);	
	} 
	catch(Macaroni::Exception & ex)
	{ 
		console.WriteLine("An error occured during Macaroni phase.");
		console.WriteLine(ex.GetSource());
		console.WriteLine(ex.GetMessage());
		return false;
	}
	catch(Macaroni::Parser::ParserException & pe)
	{
		console.WriteLine("Error parsing Macaroni code: ");
		console.WriteLine(pe.GetSource()->ToString());
		console.WriteLine(pe.GetMessage());
		return false;
	}
	return true;
}

bool Builder::CopyHeaderFiles(boost::filesystem::path headersDir)
{
	//const Path & rootDir = manifest.GetRootDirectory();
	//PathPtr headersDirPath = rootDir.NewPathForceSlash(manifest.GetCppHeadersOutput());
	//headersDirPath->CreateDirectory();

	//boost::filesystem::path headersDir(headersDirPath->GetAbsolutePath());
	
	std::vector<Path> hFiles;

	std::vector<const std::string>::iterator itr;
	for(itr = cppSrcRoots.begin(); itr != cppSrcRoots.end(); itr ++)
	{
		const std::string & srcDirectory = *itr;
		boost::filesystem::path srcDirectoryPath = 
			boost::filesystem::path(srcDirectory);
		FileSet input(srcDirectoryPath, "\\.h");
		FileSet::Iterator itr2 = input.Begin();
		FileSet::Iterator end = input.End();
		for(; itr2 != end; ++ itr2)
		{			
			Path path(srcDirectoryPath, *itr2);
			path.CopyToDifferentRootPath(headersDir);
		}	
	}

	return true;
}

void Builder::createCppSrcRoots()
{
	cppSrcRoots = manifest.GetMSource();
	cppSrcRoots.push_back(manifest.GetMOutput());
	for (unsigned int i = 0; i < cppSrcRoots.size(); i ++) 
	{
		boost::filesystem::path srcPath(cppSrcRoots[i]);
		cppSrcRoots[i] = boost::filesystem::system_complete(srcPath).string();
	}
}

void Builder::createCppFileList()
{
	cppFiles = std::vector<CppFile>();

	console.WriteLine("~ Generating listing of C++ source files. ~");
	const Path rootDir(manifest.GetRootDirectory(), manifest.GetRootDirectory());
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

void Builder::createIncludePaths(std::vector<const std::string> & includes)
{
	for (unsigned int i = 0; i < cppCompiler->GetIncludePaths().size(); i ++)
	{
		includes.push_back(cppCompiler->GetIncludePaths()[i]);
	}
	createIncludePaths(includes, configuration);
}

void Builder::createIncludePaths(std::vector<const std::string> & includes,
								 const Configuration & config)
{
	const std::vector<const std::string> & configIncludes = config.GetCInclude();
	for (unsigned int i = 0; i < configIncludes.size(); i ++)
	{
		includes.push_back(configIncludes[i]);
	}

	const std::vector<const ConfigurationId> & deps = config.GetDependencies();
	for (unsigned int i = 0; i < deps.size(); i ++)
	{
		const ConfigurationId & cId = deps[i];
		const Manifest dm(cId.GetManifestId().FindFinalManifestFile());
		const Configuration * dConfig = dm.GetConfiguration(cId.GetName());
		if (dConfig == nullptr)
		{
			std::stringstream ss;
			ss << "Could not find configuration \"" << cId.GetName() 
				<< "\" for dependency file \"" << cId.GetManifestId().FindFinalManifestFile()  << "\".";
			throw Macaroni::Exception(ss.str().c_str());
		}
		createIncludePaths(includes, *dConfig);
	}
}

bool Builder::CreateInterface()
{
	return CreateInterfaceMh();// &&
		   //CopyHeaderFiles();
}

bool Builder::CreateInterfaceMh()
{
	//TODO: Call Macaroni generator somehow.
	return true;
}

void Builder::Execute()
{
	console.WriteLine("--------------------------------------------------------------------------------");
	console.Write("Configuration: "); console.WriteLine(configuration.GetName());
	console.WriteLine("--------------------------------------------------------------------------------");

	const Path dir(manifest.GetRootDirectory(), manifest.GetRootDirectory());
	PathPtr mWork = dir.NewPathForceSlash("./MWork");
	mWork->CreateDirectory();

	PathPtr genSrc = mWork->NewPathForceSlash("/GeneratedSource");
	genSrc->CreateDirectory();

	bool success = false;
	if (CompileMacaroni())
	{
		createCppSrcRoots();
		createCppFileList();

		if (!cppCompiler || 
			(CompileCpp() && Link()))
		{
			if (CreateInterface())
			{
				if (Install())
				{
					success = true;
				}
			}
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
	boost::filesystem::path exePath(Macaroni::IO::Paths::GetExeDirectoryPath());
	boost::filesystem::path compilerDirPath = exePath / "Compilers";
	boost::filesystem::path compilerFilePath = compilerDirPath / configuration.GetCompiler();
	if (!boost::filesystem::exists(compilerFilePath))
	{
		compilerFilePath = compilerDirPath / (configuration.GetCompiler() + ".lua");
	}
	return compilerFilePath;
}

bool Builder::Install()
{
	path userPath(Paths::GetUserPath());
	path installPath = userPath / "Libraries" / manifest.GetGroup() 
						/ manifest.GetName() / manifest.GetVersion();
	if (boost::filesystem::exists(installPath))
	{
		boost::filesystem::remove_all(installPath);
	}
	else
	{
		boost::filesystem::create_directories(installPath);
	}

	// Headers-
	path headersLocal(manifest.GetCppHeadersOutput());
	path headersInstall = installPath / "Headers";
	boost::filesystem::create_directories(headersInstall);
	CopyHeaderFiles(headersInstall);

	// Interface.mh 
	path localMhInterface(manifest.GetRootDirectory());
	localMhInterface = localMhInterface / manifest.GetMOutput();
	localMhInterface = localMhInterface / "Interface.mh";
	path exportMhInterfaceDir(installPath);
	exportMhInterfaceDir = exportMhInterfaceDir / "Interface";
	boost::filesystem::create_directories(exportMhInterfaceDir);
	path exportMhInterface = exportMhInterfaceDir/ "Interface.mh";
	boost::filesystem::copy_file(localMhInterface, exportMhInterface);

	// Install manifest
	Manifest finalManifest(manifest.GetManifestFile());
	// change MSource
	std::vector<const std::string> newSrc;
	Path interfaceRelativePath(installPath, exportMhInterfaceDir);
	newSrc.push_back(interfaceRelativePath.GetRelativePath());
	finalManifest.SetMSource(newSrc);
	// Add headers to includes.
	Path headersRelativePath(installPath, headersInstall);
	const Configuration * allConfig = finalManifest.GetConfiguration("all");
	if (allConfig == nullptr) 
	{
		throw Macaroni::Exception("Could not find \"all\" configuration.");
	}
	// This is a bad programming practice; however, consider that me want do 
	// this now.
	Configuration * mutableAllConfig = const_cast<Configuration *>(allConfig);
	mutableAllConfig->GetMutableCInclude().push_back(headersRelativePath.GetRelativePath());
	// Save to different path.
	path installManifestPath = installPath / "manifest-final.lua";	
	finalManifest.SaveAs(installManifestPath); 
	//boost::filesystem::copy_file(manifest.GetManifestFile(), installManifestPath);	
	
	//Path::CopyDirectoryContents(headersLocal, headersInstall);
	return true;
}


bool Builder::Link()
{	
	console.WriteLine(
"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ LINK"
		);

	if (configuration.GetFinal().empty())
	{
		console.WriteLine("The manifest's final directory is empty!");
		return true;
	}
	
	// COMPUTE Lib paths:
	std::vector<const std::string> libPaths;	
	for (unsigned int i = 0; i < cppCompiler->GetLinkerLibraryPaths().size(); i ++)
	{
		libPaths.push_back(cppCompiler->GetLinkerLibraryPaths()[i]);
	}
	for (unsigned int i = 0; i < configuration.GetDependencies().size(); i ++)
	{		
		const ConfigurationId & dId = configuration.GetDependencies()[i];
		Manifest dm(dId.GetManifestId().FindFinalManifestFile());	
		const Configuration * dConfig = dm.GetConfiguration(dId.GetName());
		if (dConfig == nullptr)
		{
			std::stringstream ss;
			ss << "Could not find configuration \"" 
				<< dId.GetName() << " for dependency (" 
				<< dId.GetManifestId().GetGroup() << ", "
				<< dId.GetManifestId().GetName() << ", "
				<< dId.GetManifestId().GetVersion() 
				<< ").";
			throw Macaroni::Exception(ss.str().c_str());
		}
		const std::vector<const std::string> dLibs = 
			dConfig->GetLinkerLibraries();
		for (unsigned int j = 0; j < dLibs.size(); j ++)
		{
			libPaths.push_back(dLibs[j]);
		}
	}
	//end if

	std::stringstream args;
	for (unsigned int i = 0; i < libPaths.size(); i ++)
	{
		args << " /LIBPATH:\"" << libPaths[i] << "\" ";
	}

	for(std::vector<CppFile>::iterator itr = cppFiles.begin(); 
		itr != cppFiles.end(); itr ++)
	{
		CppFile & file = *itr;
		args << " \"" << file.GetObjectFilePath() << "\" ";
	}
	const Path rootDir(manifest.GetRootDirectory(), manifest.GetRootDirectory());
	PathPtr finalDirRoot = rootDir.NewPathForceSlash(manifest.GetFinalOutput());
	PathPtr dir = finalDirRoot->NewPathForceSlash(configuration.GetName());
	dir->CreateDirectory();
	PathPtr finalFile = dir->NewPathForceSlash(configuration.GetFinal());

	args << " /OUT:\"" << finalFile->GetAbsolutePath() << "\" ";

	args << configuration.GetAdditionalLinkerArgs();

	Process proc(cppCompiler->GetLinkerExe(),
				 args.str(),
				 dir->GetAbsolutePath(),
				 cppCompiler->GetPaths(),
				 cppCompiler->GetEnvironmentVariables());
	proc.Run(console);
	
	return finalFile->Exists();
}

} }

#endif
