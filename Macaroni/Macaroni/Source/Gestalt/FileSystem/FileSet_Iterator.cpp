#ifndef GESTALT_FILESYSTEM_FILESET_ITERATOR_CPP
#define GESTALT_FILESYSTEM_FILESET_ITERATOR_CPP

#include "FileSet.h"
#include "../../Macaroni/Exception.h"
#include <iostream>

BEGIN_NAMESPACE2(Gestalt, FileSystem)

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
			if (fileSet->testPath((*itr).string()))//boost::regex_search((*itr).string(), fileSet->regExp))
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

