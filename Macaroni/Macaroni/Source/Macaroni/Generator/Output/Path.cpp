#ifndef MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP
#define MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP

#include "Path.h"
#include "../../../Gestalt/FileSystem/FileSet.h"

using Gestalt::FileSystem::FileSet;

BEGIN_NAMESPACE(Macaroni, Generator, Output)

Path::Path(boost::filesystem::path rootPath, const char * path)
:rootPath(rootPath), path(path)
{
}

Path::Path(const Path & other)
:rootPath(other.rootPath), path(other.path)
{
}

Path::Path(boost::filesystem::path rootPath, boost::filesystem::path path)
:rootPath(rootPath), path(path)
{
}
	
PathListPtr Path::GetPaths()
{
	PathListPtr rtnList(new PathList());

	FileSet files(this->path);	
	for(FileSet::Iterator itr = files.Begin(); itr != files.End(); ++ itr)
	{
		Path p(this->rootPath, *itr);
		rtnList->push_back(p);	
	}

	return rtnList;
}

std::string Path::ToString()
{
	std::string fullPath(path.string());
	std::string rootPathStr = rootPath.string();
	size_t rootPathSize = rootPathStr.length();
	std::string relativePath = fullPath.substr(rootPathSize);
	return relativePath;
}
	
END_NAMESPACE

#endif

