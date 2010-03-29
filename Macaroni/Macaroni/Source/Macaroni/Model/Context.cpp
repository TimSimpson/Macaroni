#ifndef MACARONI_MODEL_CONTEXT_CPP
#define MACARONI_MODEL_CONTEXT_CPP

#include "Context.h"
#include "Library.h"
#include "Node.h"

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

LibraryPtr Context::CreateLibrary(const std::string & name, const std::string & version)
{
	libraries.push_back(new Library(this, name, version));
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
