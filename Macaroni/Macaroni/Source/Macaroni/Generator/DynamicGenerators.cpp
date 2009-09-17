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

void runDynamicGenerator(ContextPtr context, const path & rootPath, const path & filePath)
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
		runDynamicGenerator(context, rootPath, p);
	}
}

END_NAMESPACE2

#endif

