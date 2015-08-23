/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_IO_PATH_CPP
#define MACARONI_IO_PATH_CPP

#include "Path.h"
#include <boost/filesystem/convenience.hpp>
#include <boost/foreach.hpp>
#include <Macaroni/Platform/FileTime.h>
#include <boost/format.hpp>
#include <Macaroni/Exception.h>
#include <Macaroni/IO/GeneratedFileWriter.h>
#include <iostream>
#include <Macaroni/Environment/Values/Macaroni_IO_Path.h>
#include <Macaroni/IO/RegexFileSet.h>
#include <Macaroni/IO/RegexFileSetIterator.h>
#include <sstream>
#include <Macaroni/StringException.h>

using Macaroni::Platform::FileTime;
using boost::format;
using Macaroni::IO::RegexFileSet;
using Macaroni::IO::RegexFileSetIterator;

BEGIN_NAMESPACE2(Macaroni, IO)

Macaroni::Environment::Values::AnyPtr ToAny(PathPtr p)
{
	Macaroni::Environment::Values::AnyPtr any(
		new Macaroni::Environment::Values::Macaroni_IO_Path(p));
	return any;
}

//namespace {
//	static path createPath(const std::string & absolutePath)
//	{
//		path p = absolutePath;
//	}
//}
Path::Path(const std::string & absolutePath)
:	absolutePath(boost::none), path(absolutePath), rootPath(absolutePath)
{
}

Path::Path(const boost::filesystem::path & rootPath)
:	absolutePath(boost::none), path(rootPath), rootPath(rootPath)
{
}

Path::Path(const boost::filesystem::path & rootPath, const char * path)
:	absolutePath(boost::none), path(path), rootPath(rootPath)
{
	assertPathExistsInRootPath();
}

Path::Path(const Path & other)
:	absolutePath(other.absolutePath), path(other.path), rootPath(other.rootPath)
{
	assertPathExistsInRootPath();
}

Path::Path(const boost::filesystem::path & rootPath,
		   const boost::filesystem::path & path)
:path(path), rootPath(rootPath)
{
	assertPathExistsInRootPath();
}

bool Path::operator==(const Path & other) const
{
	return path == other.path && rootPath == other.rootPath;
}

bool Path::operator!=(const Path & other) const
{
	return path != other.path || rootPath != other.rootPath;
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
		throw Macaroni::StringException(msg.c_str());
	}
}

void Path::ClearDirectoryContents(bool throw_if_not_exists)
{
	if (!Exists())
	{
		if (throw_if_not_exists)
		{
			std::stringstream ss;
			ss << "Can't clear directory contents from path \"" << path.string()
		   	   << "\" as it does not exist.";
			std::string msg(ss.str());
			throw Macaroni::StringException(msg.c_str());
		}
	}
	else if (!IsDirectory())
	{
		std::stringstream ss;
		ss << "Cannot clear directory contents from path \"" << path.string()
		   << " as it is not a directory.";
		std::string msg(ss.str());
		throw Macaroni::StringException(msg.c_str());
	}
	else
	{
		boost::filesystem::remove_all(path);
	}
}

void Path::CopyDirectoryContents(boost::filesystem::path & bSrc,
								 boost::filesystem::path & bDst)
{
	Path src(bSrc, bSrc);
	CopyDirectoryContents(src, bDst);
}

void Path::CopyDirectoryContents(Path & src,
								 Path & dst)
{
	boost::filesystem::path p_dst(dst.GetAbsolutePath());
	CopyDirectoryContents(src, p_dst);
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

void Path::CopyTo(boost::filesystem::path newPath, bool overrideIfExist)
{
	if (boost::filesystem::exists(newPath))
	{
		if (overrideIfExist)
		{
			boost::filesystem::remove(newPath);
		}
	}
	boost::filesystem::copy_file(this->path, newPath);
}

void Path::CopyToDifferentRootPath(boost::filesystem::path newRootPath,
								   bool overrideIfExist)
{
	boost::filesystem::path dstPath = newRootPath / GetRelativePath();
	//std::cio << "Creating directories at " .. dstPath.branch_path() << std::endl;
	boost::filesystem::create_directories(dstPath.branch_path());
	//boost::filesystem::remove(dstPath);

	if (boost::filesystem::exists(dstPath))
	{
		if (overrideIfExist)
		{
			boost::filesystem::remove(dstPath);
		}
	}
	boost::filesystem::copy_file(this->path, dstPath);
}

void Path::CopyToDifferentRootPath(const PathPtr & rootPath,
								   bool overrideIfExist)
{
	boost::filesystem::path rp(rootPath->GetAbsolutePath());
	this->CopyToDifferentRootPath(rp, overrideIfExist);
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

GeneratedFileWriterPtr Path::CreateFile(bool trackFormatInfo) const
{
	try
	{
		//boost::filesystem::path absolute = boost::filesystem::system_complete(path);
		return GeneratedFileWriterPtr(
			new GeneratedFileWriter(*this, trackFormatInfo)
		);
	}
	catch(...)
	{
		std::stringstream ss;
		ss << "Can't create file : " << path.string();
		std::string msg(ss.str());
		throw Macaroni::StringException(msg.c_str());
	}
}

PathPtr Path::CreateWithCurrentAsRoot() const
{
	PathPtr newPath(new Path(boost::filesystem::system_complete(path)));
	return newPath;
}

PathPtr Path::CreateWithDifferentRootPath(const PathPtr & otherPath)
{
	boost::filesystem::path p = otherPath->rootPath / GetRelativePath();
	PathPtr path(new Path(otherPath->rootPath, p));
	return path;
}

PathPtr Path::DifferentSuffix(const std::string & suffix)
{
	std::stringstream ss;
	ss << boost::filesystem::system_complete(path).stem().string() << suffix;
	boost::filesystem::path p = path.parent_path() / ss.str();
	PathPtr newPath(new Path(this->rootPath, p));
	return newPath;
}

bool Path::Exists() const
{
	return boost::filesystem::exists(this->path);
}

std::string Path::GetAbsolutePath() const
{
	if (!absolutePath)
	{
		// In Windows, system_complete can be used successfully in most cases.
		// In Linux there seems to be multiple problems with it.
		// However, this program (like C++ itself) may often use forward
		// slashes in Windows. "canonical" throws on forward slashes, in which
		// case we refer to system_complete, which is what was used everywhere
		// anyway when Macaroni only ran on Windows.
		auto noncost_this = const_cast<Path *>(this);
		try
		{

			noncost_this->absolutePath
				= boost::filesystem::canonical(path).string();
		}
		catch(...)
		{
			try
			{
				noncost_this->absolutePath
					= boost::filesystem::system_complete(path).string();
			}
			catch(...)
			{
				std::cerr << "PATH ERROR: File " << path.string()
			    	      << " has no canonical path." << std::endl;
				throw;
			}
		}
		// NOTE: canonical throws errors if a Path isn't found, but it seems
		// that Path instances are created before a Path exists. In Linux I
		// believe I saw issues using system_complete, but for now I'll return
		// to using it.
	}
	return absolutePath.get();
}

std::string Path::GetAbsolutePathForceSlash() const
{
	std::string absPath = GetAbsolutePath();
	return NormalizePathString(absPath);
}

std::string Path::GetFileName() const
{
	/*if (!IsRegularFile())
	{
		std::stringstream ss;
		ss << "Cannot change the extension as the given path \""
			<< GetAbsolutePath()
			<< "\" is not a regular file.";
		throw Macaroni::Exception(ss.str().c_str());
	}*/
	return path.filename().string();
}

std::string Path::GetFileNameWithoutExtension() const
{
	return path.filename().stem().string();
}

PathPtr Path::GetParentPath() const
{
	PathPtr rtn(new Path(rootPath, path.parent_path()));
	return rtn;
}

PathListPtr Path::GetPaths() const
{
	return GetPaths("");
}

PathListPtr Path::GetPaths(const std::string & matchingPattern) const
{
	PathListPtr rtnList(new PathList());

	RegexFileSet files(this->path, matchingPattern);
	for(RegexFileSetIterator itr = files.Begin(); itr != files.End(); ++ itr)
	{
		PathPtr p(new Path(this->rootPath, *itr));
		rtnList->push_back(p);
	}

	return rtnList;
}

PathPtr Path::GetRoot() const
{
	PathPtr rtn(new Path(rootPath));
	return rtn;
}

/** Creates a string to get us from the relative path back to root. */
std::string Path::GetStringBackToRoot() const
{
	boost::filesystem::path p = path;
	std::stringstream ss;
	bool first = true;
	while(p != "" && p != rootPath)
	{
		p = p.parent_path();
		if (!first)
		{
			ss << "/";
		}
		else
		{
			first = false;
		}
		ss << "..";
	}
	return ss.str();
}

bool Path::IsDirectory() const
{
	return boost::filesystem::is_directory(path);
}

bool Path::IsFileOlderThan(const std::string & filePath) const
{
	if (filePath.size() < 1)
	{
		throw Macaroni::StringException("Argument \"filePath\" cannot be empty.");
	}
	if (!this->Exists())
	{
		return true; // FoR WHAT BE MORE ANCIENT THAN THE VOID OF
					 // ETERNAL NOTHINGNESS?!!
	}
	boost::filesystem::path one(this->GetAbsolutePath());
	boost::filesystem::path two(filePath);

	return FileTime::File1IsOlderThanFile2(one, two);
}

bool Path::IsRegularFile() const
{
	return boost::filesystem::is_regular_file(path);
}

PathPtr Path::NewPath(const std::string & name) const
{
	std::string newStr(this->path.string() + name);
	boost::filesystem::path newPath(newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		throw Macaroni::StringException("Illegal directory.");
	}
	return PathPtr(new Path(this->rootPath, newPath));
}

PathPtr Path::NewPathForceSlash(const std::string & name) const
{
	//std::string newStr(this->path.string() + name);
	boost::filesystem::path newPath(this->path / name);////newStr);
	if (!stringBeginsWith(newPath.string(), this->rootPath.string()))
	{
		std::stringstream ss;
		ss << "Illegal directory ("
			<< newPath.string()
			<< ".  Must begin with the path "
			<< this->rootPath.string()
			<< ".";
		throw Macaroni::StringException(ss.str().c_str());
	}
	return PathPtr(new Path(this->rootPath, newPath));
}

std::string Path::NormalizePathString(std::string & original)
{
	//const char seperator = boost::filesystem::path_alt_separator<boost::filesystem::path>::value;
	//TODO: The line above worked in 1.42.0 but doesn't in 1.45.0.  Find
	// an actual way to do this.
	const char seperator = '\\';
	if (!(seperator == '/'))
	{
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
		return original;
	}
}

void Path::RenameRelative(const std::string & relativePath)
{
	boost::filesystem::path newPath = rootPath / relativePath;
	boost::filesystem::rename(path, newPath);
	this->path = newPath;
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

std::string Path::GetRelativePathNormalized() const
{
	std::string relPath = GetRelativePath();
	const std::string original = NormalizePathString(relPath);
	unsigned int index = 0;
	while(original.length() > index && original[index] == '/')
	{
		++ index;
	}
	if (index > 0)
	{
		return original.substr(index);
	}
	return original;
}

std::string Path::ToString() const
{
	return str(format("%s + %s") % GetAbsolutePath() % GetRelativePath());
}


END_NAMESPACE2

#endif

