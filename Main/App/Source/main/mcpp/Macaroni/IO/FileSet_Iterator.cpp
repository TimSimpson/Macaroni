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
#ifdef MACARONI_IO_FILESET_ITERATOR_CPP
#define MACARONI_IO_FILESET_ITERATOR_CPP

#include "FileSet.h"
#include "../Exception.h"
#include <iostream>

BEGIN_NAMESPACE2(Macaroni, IO)

using boost::filesystem::directory_iterator;
using boost::filesystem::path;

FileSet::Iterator::Iterator(const FileSet * parent)
: fileSet(parent), iterateSingleFile(false), stack()
{
	if (fileSet != nullptr)
	{
		if (is_directory(fileSet->root))
		{
			stack.push_back(directory_iterator(fileSet->root));
			iterate(true);
		}
		else
		{
			std::string path(fileSet->root.string());
			if (fileSet->testPath(path))
			{
				iterateSingleFile = true;
			}
			else
			{
				fileSet = nullptr;
			}
		}
	}
}

bool FileSet::Iterator::operator==(const FileSet::Iterator& other) const
{
	if (&other == this)
	{
		return true;
	}
	if (this->fileSet == nullptr || other.fileSet == nullptr)
	{
		return this->fileSet == other.fileSet;
	}
	if (other.stack.size() != this->stack.size())
	{
		return false;
	}
	for(size_t i = 0; i < stack.size(); i ++)
	{
		if (other.stack[i] != stack[i])
		{
			return false;
		}
	}
	return true;
}

path FileSet::Iterator::operator*()
{
	if (iterateSingleFile)
	{
		return fileSet->root;
	}
	if (finished())
	{
		throw Macaroni::Exception("FileSet iterator is finished.");
	}
	directory_iterator & itr = stack.back();
	boost::filesystem::path path = *itr;
	return path;
}

FileSet::Iterator FileSet::Iterator::operator++()
{
	using boost::filesystem::is_directory;
	if (iterateSingleFile)
	{
		iterateSingleFile = false;
		fileSet = nullptr;
		return *this;
	}
	if (finished())
	{
		throw Macaroni::Exception("Index out of bounds.");
	}
	iterate(false);
	return *this;
//	using boost::filesystem::is_directory;
//	if (finished())
//	{
//		throw Gestalt::RuntimeException("Index out of bounds.");
//	}
//	directory_iterator end_itr;
//	bool freshItr = false;
//	while(stack.size() > 0)
//	{
//		directory_iterator & itr = stack.back();
//		int stackAdds = 0;
//		while(itr != end_itr)
//		{
//			if (!freshItr)
//			{
//				itr ++;
//			}
//			if (itr != end_itr && true)
//			{
//				if (is_directory(itr->status()))
//				{
//					stack.push_back(directory_iterator(itr));
//					freshItr = true;
//					break;
//				}
//				else if (true)//boost::regex_match((*itr).string(), fileSet->regExp))
//				{
//					return *this;
//				}
//			}
//		}
//		if (itr == end_itr)
//		{
//			stack.pop_back();
//		}
//	}
//	fileSet = nullptr;
//	return *this;
}

bool FileSet::Iterator::finished()
{
	if (fileSet == nullptr)
	{
		MACARONI_ASSERT(stack.size() < 1, "Stack size < 1, yet fileset is null.");
		return true;
	}
	return false;
}

void FileSet::Iterator::iterate(bool freshItr)
{
	using boost::filesystem::is_directory;
	directory_iterator end_itr;
	while(stack.size() > 0)
	{
		directory_iterator & itr = stack.back();

		if (!freshItr)
		{
			itr ++;
		}
		freshItr = false;

		if (itr == end_itr)
		{
			stack.pop_back();
		}
		else if (is_directory(itr->status()))
		{
			stack.push_back(directory_iterator(*itr));
			freshItr = true;
			continue;
		}
		else if (true)
		{
			if (fileSet->testPath((*itr).path().string()))//boost::regex_search((*itr).string(), fileSet->regExp))
			{
				return;
			}
		}
	}
	fileSet = nullptr;
	return;
}

END_NAMESPACE2

#endif

