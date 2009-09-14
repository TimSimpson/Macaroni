#ifndef MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP
#define MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP

#include "Path.h"
#include "../../../Gestalt/FileSystem/FileSet.h"

using Gestalt::FileSystem::FileSet;

BEGIN_NAMESPACE(Macaroni, Generator, Output)

Path::Path(const char * path)
:path(path)
{
}

Path::Path(const Path & other)
:path(other.path)
{
}

Path::Path(boost::filesystem::path path)
:path(path)
{
}
	
PathListPtr Path::GetPaths()
{
	PathListPtr rtnList(new PathList());

	FileSet files(this->path);	
	for(FileSet::Iterator itr = files.Begin(); itr != files.End(); ++ itr)
	{
		Path p(*itr);
		rtnList->push_back(p);	
	}

	return rtnList;
}

std::string Path::ToString()
{
	return path.string();
}
	
END_NAMESPACE

#endif

