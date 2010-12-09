#ifndef MACARONI_IO_PATH_CPP
#define MACARONI_IO_PATH_CPP

#include "Path.h"
#include <boost/filesystem/convenience.hpp>
#include <boost/foreach.hpp>
#include "../Platform/Windows/FileTime.h"
#include "../IO/FileSet.h"
#include "../Exception.h"
#include <iostream>
#include <sstream>

using Macaroni::Platform::Windows::FileTime;
using Macaroni::IO::FileSet;

BEGIN_NAMESPACE2(Macaroni, IO)

Path::Path(boost::filesystem::path rootPath)
:rootPath(rootPath), path(rootPath)
{
}

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

void Path::CopyDirectoryContents(boost::filesystem::path & bSrc,
								 boost::filesystem::path & bDst)
{
	Path src(bSrc, bSrc);
	CopyDirectoryContents(src, bDst);
}

void Path::CopyDirectoryContents(Path & src,
								 boost::filesystem::path & bDst)
{ 
	PathListPtr paths = src.GetPaths();
	for(unsigned int i = 0; i < paths->size(); i ++)
	{ 
		PathPtr p = (*paths)[i];
		if (!p->IsDirectory())
		{
			p->CopyToDifferentRootPath(bDst);
		} 
		else
		{
			CopyDirectoryContents(*p, bDst);
		}
	}
}

void Path::CopyToDifferentRootPath(boost::filesystem::path newRootPath)
{
	boost::filesystem::path dstPath = newRootPath / GetRelativePath();
	//std::cio << "Creating directories at " .. dstPath.branch_path() << std::endl;
	boost::filesystem::create_directories(dstPath.branch_path());
	//boost::filesystem::remove(dstPath);
	
	boost::filesystem::copy_file(this->path, dstPath);	
}

void Path::CopyToDifferentRootPath(const PathPtr & rootPath)
{
	boost::filesystem::path rp(rootPath->GetAbsolutePath());
	this->CopyToDifferentRootPath(rp);	
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

GeneratedFileWriterPtr Path::CreateFile() const
{
	boost::filesystem::path absolute = boost::filesystem::system_complete(path);
	return GeneratedFileWriterPtr(new GeneratedFileWriter(absolute));
}

bool Path::Exists() const
{
	return boost::filesystem::exists(this->path);
}

std::string Path::GetAbsolutePath() const
{
	return boost::filesystem::system_complete(path).string();
}

std::string Path::GetAbsolutePathForceSlash() const
{
	//const char seperator = boost::filesystem::path_alt_separator<boost::filesystem::path>::value;
	//TODO: The line above worked in 1.42.0 but doesn't in 1.45.0.  Find
	// an actual way to do this.
	const char seperator = '\\'; 
	if (!seperator == '/')
	{
		std::string original = GetAbsolutePath();
		std::stringstream ss;
		BOOST_FOREACH(char c, original)
		{
			if (c == seperator)
			{
				ss << "/";
			}
			else 
			{
				ss << c;
			}
		}
		return ss.str();
	}
	else
	{
		return GetAbsolutePath();
	}
}

PathListPtr Path::GetPaths() const
{
	return GetPaths("");
}

PathListPtr Path::GetPaths(const std::string & matchingPattern) const
{
	PathListPtr rtnList(new PathList());

	FileSet files(this->path, matchingPattern);	
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

bool Path::IsFileOlderThan(const std::string & filePath) const
{
	if (!this->Exists())
	{
		return true; // FoR WHAT BE MORE ANCIENT THAN THE VOID OF 
					 // ETERNAL NOTHINGNESS?!!
	} 
	boost::filesystem::path one(this->GetAbsolutePath());
	boost::filesystem::path two(filePath);
	
	return FileTime::File1IsOlderThanFile2(one, two);	
}

PathPtr Path::NewPath(const std::string & name) const
{	 
	std::string newStr(this->path.string() + name);	
	boost::filesystem::path newPath(newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		throw Macaroni::Exception("Illegal directory.");
	}
	return PathPtr(new Path(this->rootPath, newPath));
}

PathPtr Path::NewPathForceSlash(const std::string & name) const
{	
	//std::string newStr(this->path.string() + name);	
	boost::filesystem::path newPath(this->path / name);////newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		throw Macaroni::Exception("Illegal directory.");
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

std::string Path::GetRelativePath() const
{
	std::string fullPath(path.string());
	std::string rootPathStr = rootPath.string();
	size_t rootPathSize = rootPathStr.length();
	std::string relativePath = fullPath.substr(rootPathSize);
	return relativePath;
}

std::string Path::ToString() const
{
	return GetRelativePath();
}
	
END_NAMESPACE2

#endif

