#ifndef MACARONI_MODEL_CONTEXT_CPP
#define MACARONI_MODEL_CONTEXT_CPP

#include "Context.h"
#include "Library.h"
#include "Node.h"
#include <boost/foreach.hpp>

using Macaroni::Build::LibraryId;

BEGIN_NAMESPACE2(Macaroni, Model)

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

LibraryPtr Context::FindOrCreateLibrary(const std::string & group, 
								  		const std::string & name, 
								  		const std::string & version)
{
	const LibraryId id(group, name, version);
	return FindOrCreateLibrary(id);
}

LibraryPtr Context::FindOrCreateLibrary(const Macaroni::Build::LibraryId & id)
{
	BOOST_FOREACH(Library * lib, libraries)
	{
		if (lib->GetId() == id)
		{
			return LibraryPtr(lib);
		}
	}
	libraries.push_back(new Library(this, id));
	Library * newLib = libraries.back();
	return LibraryPtr(newLib);
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
