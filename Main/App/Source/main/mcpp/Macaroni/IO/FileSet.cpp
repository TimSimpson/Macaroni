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
#ifndef MACARONI_IO_FILESET_CPP
#define MACARONI_IO_FILESET_CPP

#include "FileSet.h"

using boost::filesystem::path;

BEGIN_NAMESPACE2(Macaroni, IO)

FileSet::FileSet(const path & rootPath)
:regExp("."), root(rootPath)
{
}

FileSet::FileSet(const path & rootPath, const char * regExToMatch)
:regExp(regExToMatch), root(rootPath)
{
}

FileSet::FileSet(const path & rootPath, std::string regExToMatch)
:regExp(regExToMatch), root(rootPath)
{
}

FileSet::FileSet(const path & rootPath, boost::regex regExp)
:regExp(regExp), root(rootPath)
{
}

FileSet::FileSet(const FileSet & other)
:regExp(other.regExp), root(other.root)
{
}

void FileSet::operator=(const FileSet & other)
{
	this->regExp = other.regExp;
	this->root = other.root;
}


FileSet::Iterator FileSet::Begin() const
{
	return FileSet::Iterator(this);
}

FileSet::Iterator FileSet::End() const
{
	return Iterator(nullptr);
}

boost::filesystem::path FileSet::GetRoot() const
{
	return root;
}


bool FileSet::testPath(const std::string & str) const
{
	return boost::regex_search(str, this->regExp);
}


END_NAMESPACE2

#endif

