#ifndef MACARONI_IO_PATH_CPP
#define MACARONI_IO_PATH_CPP

#include "Path.h"
#include <boost/filesystem/convenience.hpp>
#include "../../Gestalt/FileSystem/FileSet.h"
#include "../Exception.h"
#include <iostream>
#include <sstream>

using Gestalt::FileSystem::FileSet;

BEGIN_NAMESPACE2(Macaroni, IO)


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

void Path::CreateDirectory() const
{
	if (Exists())
	{
		return;
	}	
	boost::filesystem::create_directories(path);
	//boost::filesystem::create_directory(path);	
}

bool Path::Exists() const
{
	return boost::filesystem::exists(this->path);
}

std::string Path::GetAbsolutePath() const
{
	return boost::filesystem::system_complete(path).string();
}

PathListPtr Path::GetPaths() const
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

bool Path::IsDirectory() const
{
	return boost::filesystem::is_directory(path);
}

PathPtr Path::NewPath(const std::string & name) const
{	
	std::string newStr(this->path.string() + name);	
	boost::filesystem::path newPath(newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		throw new Macaroni::Exception("Illegal directory.");
	}
	return PathPtr(new Path(this->rootPath, newPath));
}

PathPtr Path::NewPathForceSlash(const std::string & name) const
{	
	//std::string newStr(this->path.string() + name);	
	boost::filesystem::path newPath(this->path / name);////newStr);
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

std::string Path::ToString() const
{
	std::string fullPath(path.string());
	std::string rootPathStr = rootPath.string();
	size_t rootPathSize = rootPathStr.length();
	std::string relativePath = fullPath.substr(rootPathSize);
	return relativePath;
}
	
END_NAMESPACE2

#endif
