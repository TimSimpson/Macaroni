#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_CPP
#define MACARONI_GENERATOR_DYNAMICGENERATORS_CPP

#include <boost/foreach.hpp>
#include "DynamicGenerators.h"
#include "Lua/DynamicGenerator.h"
#include "../IO/FileSet.h"
#include <Macaroni/Build/BuildContext.h>
#include "../Model/Library.h"
#include <iostream>
#include "../IO/Path.h"
#include "../IO/Paths.h"
#include <sstream>

using Macaroni::Model::ContextPtr;
using Macaroni::IO::FileSet;
using Macaroni::Build::BuildContext;
using Macaroni::Build::BuildContextPtr;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using boost::filesystem::path;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::Environment::StringPair;

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
	std::vector<StringPair> pairs;
	std::vector<PathPtr> sources; // null
	PathPtr outputPath(new Path(rootPath, rootPath));
	PathPtr installPath; // null
	BuildContextPtr buildContext(new BuildContext(
		library, sources, outputPath, installPath, "{}"
		));
	RunDynamicGenerator(filePath, buildContext, "Generate", pairs);
}

std::string RunDynamicGenerator(const path & filePath, Macaroni::Build::BuildContextPtr buildContext,
						 const std::string & methodName,
						 const std::vector<StringPair> & arguments)
{
	Lua::DynamicGenerator file(filePath, buildContext, arguments);
	return file.Run(methodName);
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

