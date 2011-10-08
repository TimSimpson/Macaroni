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
#ifndef MACARONI_MODEL_CONTEXT_CPP
#define MACARONI_MODEL_CONTEXT_CPP

#include "Context.h"
#include "Library.h"
#include "Node.h"
#include <boost/foreach.hpp>

using Macaroni::Build::LibraryId;

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(Context * p)
{
	p->onAddReference();
}

void intrusive_ptr_release(Context * p)
{
	p->onReleaseReference();
}

Context::Context(const std::string & rootName)
:hasBeenDeleted(false),
 libraries(),
 referenceCount(0), 
 root(nullptr)
{
	root = new Node(*this, rootName);
	/*root->context = this;
	 */
	libraries.push_back(new Library(this, "%ROOT%", ""));
}
	
Context::~Context()
{
	MACARONI_ASSERT(referenceCount == 0, "Ref count != 0 on Context destruction!");
	delete root;
	for (unsigned int i = 0; i < libraries.size(); i ++)
	{
		delete libraries[i];
	}
	hasBeenDeleted = true;
}

LibraryPtr Context::FindLibrary(const std::string & group, 
						  		const std::string & name, 
						  		const std::string & version)
{
	Macaroni::Build::LibraryId id(group, name, version);
	return FindLibrary(id);
}

LibraryPtr Context::FindLibrary(const Macaroni::Build::LibraryId & id)
{
	BOOST_FOREACH(Library * lib, libraries)
	{
		if (lib->GetId() == id)
		{
			return LibraryPtr(lib);
		}
	}
	return LibraryPtr();
}

LibraryPtr Context::FindOrCreateLibrary(const std::string & group, 
								  		const std::string & name, 
								  		const std::string & version)
{
	const LibraryId id(group, name, version);
	boost::optional<boost::filesystem::path> none;
	return FindOrCreateLibrary(none, id);
}

LibraryPtr Context::FindOrCreateLibrary(const boost::optional<boost::filesystem::path> & installPath,
										const Macaroni::Build::LibraryId & id)
{
	LibraryPtr rtn = FindLibrary(id);
	if (!rtn)
	{
		libraries.push_back(new Library(this, id, installPath));
		rtn.reset(libraries.back());
	}
	return rtn;
}

int Context::GetReferenceCount() const
{
	return referenceCount;
}

NodePtr Context::GetRoot()
{
	return NodePtr(root);
}

LibraryPtr Context::GetRootLibrary()
{
	Library * lib = libraries[0];
	return LibraryPtr(lib);
}

void Context::onAddReference()
{
	referenceCount ++;
}

void Context::onReleaseReference()
{
	referenceCount --;
	if (referenceCount == 0)
	{
		delete this;
	}
}

END_NAMESPACE2

#endif
