#ifndef MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP
#define MACARONI_GENERATOR_OUTPUT_DIRECTORY_CPP

#include "Path.h"
#include "../../../Gestalt/FileSystem/FileSet.h"
#include "../../Exception.h"
#include <sstream>

using Gestalt::FileSystem::FileSet;

BEGIN_NAMESPACE(Macaroni, Generator, Output)


Path::Path(boost::filesystem::path rootPath, const char * path)
:rootPath(rootPath), path(path)
{
	assertPathExistsInRootPath();
}

Path::Path(const Path & other)
:rootPath(other.rootPath), path(other.path)
{
	assertPathExistsInRootPath();
}

Path::Path(boost::filesystem::path rootPath, boost::filesystem::path path)
:rootPath(rootPath), path(path)
{
	assertPathExistsInRootPath();
}

void Path::assertPathExistsInRootPath()
{
	if (!stringBeginsWith(this->path.string(), this->rootPath.string()))
	{
		std::stringstream ss;
		ss << "Illegal directory : ";
		ss << path.string() ;
		ss << " must exist within root directory ";
		ss << rootPath.string();
		ss << ".";
		std::string msg(ss.str());
		throw Macaroni::Exception(msg.c_str());
	}
}

void Path::CreateDirectory()
{
	if (Exists())
	{
		return;
	}
	boost::filesystem::create_directory(path);
}

bool Path::Exists()
{
	return boost::filesystem::exists(this->path);
}

std::string Path::GetAbsolutePath()
{
	return path.string();
}

PathListPtr Path::GetPaths()
{
	PathListPtr rtnList(new PathList());

	FileSet files(this->path);	
	for(FileSet::Iterator itr = files.Begin(); itr != files.End(); ++ itr)
	{
		PathPtr p(new Path(this->rootPath, *itr));
		rtnList->push_back(p);	
	}

	return rtnList;
}

bool Path::IsDirectory()
{
	return boost::filesystem::is_directory(path);
}

PathPtr Path::NewPath(const std::string & name)
{	
	std::string newStr(this->path.string() + name);	
	boost::filesystem::path newPath(newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		throw new Macaroni::Exception("Illegal directory.");
	}
	return PathPtr(new Path(this->rootPath, newPath));
}

bool Path::stringBeginsWith(const std::string & str, const std::string begins)
{
	if (str.length() < begins.length())
	{
		return false;
	}
	std::string sub = str.substr(0, begins.length());
	return (sub == begins);
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

