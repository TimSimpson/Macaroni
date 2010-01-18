#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_CPP
#define MACARONI_GENERATOR_DYNAMICGENERATORS_CPP

#include "DynamicGenerators.h"
#include "Lua/DynamicGenerator.h"
#include "../IO/FileSet.h"
#include "../Model/Library.h"
#include <iostream>
#include "../IO/Paths.h"
#include <sstream>

using Macaroni::Model::ContextPtr;
using Macaroni::IO::FileSet;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using boost::filesystem::path;

BEGIN_NAMESPACE2(Macaroni, Generator)

path GetGeneratorsPath()
{
	std::string exePath = Macaroni::IO::Paths::GetExeDirectoryPath();
	std::stringstream finalFilePath;
	finalFilePath << exePath << "Generators/";

	path generatorPath(finalFilePath.str());
	return generatorPath;
}

path ResolveGeneratorPath(const std::vector<Macaroni::IO::FileSet> & srcDirs, 
						  const std::string & name)
{	
	///*FileSet::Iterator end = localDirs.End();
	//for(FileSet::Iterator itr = localDirs.Begin(); itr != end; ++ itr)
	//{	
	//	path p = *itr;
	//	path guess = p / name;
	//	if (boost::filesystem::exists(guess))
	//	{
	//		return guess;
	//	}
	//}*/
	for (unsigned int i = 0; i < srcDirs.size(); i ++)
	{
		const FileSet & localDir = srcDirs[i];
		path guess = localDir.GetRoot() / name;
		if (boost::filesystem::exists(guess))
		{
			return guess;
		}
	}

	path referencePath = GetGeneratorsPath() / name;
	if (boost::filesystem::exists(referencePath))
	{
		return referencePath;
	}

	return path("");
}

void RunDynamicGenerator(LibraryPtr library, const path & rootPath, const path & filePath)
{
	Lua::DynamicGenerator file(library, rootPath, filePath);
	file.Run();
}

void RunDynamicGenerators(LibraryPtr library, const path & rootPath)
{
	std::string exePath = Macaroni::IO::Paths::GetExeDirectoryPath();	
	std::stringstream finalFilePath;
	finalFilePath << exePath << "Generators/";

	path generatorPath(finalFilePath.str());
	FileSet generators(generatorPath);
	
	FileSet::Iterator end = generators.End();
	for(FileSet::Iterator itr = generators.Begin(); itr != end; ++ itr)
	{	
		path p = *itr;
		std::cout << p.string() << "\n";
		RunDynamicGenerator(library, rootPath, p);
	}
}

END_NAMESPACE2

#endif

