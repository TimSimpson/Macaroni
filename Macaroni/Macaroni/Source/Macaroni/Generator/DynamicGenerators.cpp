#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_CPP
#define MACARONI_GENERATOR_DYNAMICGENERATORS_CPP

#include <boost/foreach.hpp>
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

path ResolveGeneratorPath(const std::vector<const std::string> & srcDirs, 
						  const std::string & name)
{
	std::stringstream ss;
	ss << name << ".lua";
	std::string name2(ss.str());

	BOOST_FOREACH(const std::string dir, srcDirs)
	{
		path dirPath(dir);	
		path guess = dirPath / name;
		if (boost::filesystem::exists(guess) && boost::filesystem::is_regular(guess))
		{
			return guess;
		}				
		path guess2 = dirPath / name2;
		if (boost::filesystem::exists(guess2) && boost::filesystem::is_regular(guess2))
		{
			return guess2;
		}
	}
	path referencePath = GetGeneratorsPath() / name;
	if (boost::filesystem::exists(referencePath) && boost::filesystem::is_regular(referencePath))
	{
		return referencePath;
	}
	path referencePath2 = GetGeneratorsPath() / name2;
	if (boost::filesystem::exists(referencePath2) && boost::filesystem::is_regular(referencePath2))
	{
		return referencePath2;
	}

	return path("");
}

path ResolveGeneratorPath(const std::vector<Macaroni::IO::FileSet> & srcDirs, 
						  const std::string & name)
{	
	std::vector<const std::string> dirs;
	for (unsigned int i = 0; i < srcDirs.size(); i ++)
	{
		const FileSet & localDir = srcDirs[i];
		dirs.push_back(localDir.GetRoot().string());
	}
	return ResolveGeneratorPath(dirs, name);
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

