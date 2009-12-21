#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_CPP
#define MACARONI_GENERATOR_DYNAMICGENERATORS_CPP

#include "DynamicGenerators.h"
#include "Lua/DynamicGenerator.h"
#include "../Gestalt/FileSystem/FileSet.h"
#include <iostream>
#include "../../Gestalt/FileSystem/Paths.h"
#include <sstream>

using Macaroni::Model::ContextPtr;
using Gestalt::FileSystem::FileSet;
using boost::filesystem::path;

BEGIN_NAMESPACE2(Macaroni, Generator)

path GetGeneratorsPath()
{
	std::string exePath = Gestalt::FileSystem::Paths::GetExeDirectoryPath();
	std::stringstream finalFilePath;
	finalFilePath << exePath << "Generators/";

	path generatorPath(finalFilePath.str());
	return generatorPath;
}

path ResolveGeneratorPath(const Gestalt::FileSystem::FileSet & localDir, 
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
	path guess = localDir.GetRoot() / name;
	if (boost::filesystem::exists(guess))
	{
		return guess;
	}

	path referencePath = GetGeneratorsPath() / name;
	if (boost::filesystem::exists(referencePath))
	{
		return referencePath;
	}

	return path("");
}

void RunDynamicGenerator(ContextPtr context, const path & rootPath, const path & filePath)
{
	Lua::DynamicGenerator file(context, rootPath, filePath);
	file.Run();
}

void RunDynamicGenerators(ContextPtr context, const path & rootPath)
{
	std::string exePath = Gestalt::FileSystem::Paths::GetExeDirectoryPath();	
	std::stringstream finalFilePath;
	finalFilePath << exePath << "Generators/";

	path generatorPath(finalFilePath.str());
	FileSet generators(generatorPath);
	
	FileSet::Iterator end = generators.End();
	for(FileSet::Iterator itr = generators.Begin(); itr != end; ++ itr)
	{	
		path p = *itr;
		std::cout << p.string() << "\n";
		RunDynamicGenerator(context, rootPath, p);
	}
}

END_NAMESPACE2

#endif

