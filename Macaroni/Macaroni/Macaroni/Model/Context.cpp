#ifndef MACARONI_MODEL_CONTEXT_CPP
#define MACARONI_MODEL_CONTEXT_CPP

#include "Context.h"
#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Context::Context(const std::string & rootName)
:referenceCount(0), root(nullptr)
{
	root = new Node(nullptr, rootName);
	root->context = this;
}
	
Context::~Context()
{
	MACARONI_ASSERT(referenceCount == 0, "Ref count != 0 on Context destruction!");
	delete root;

}

int Context::GetReferenceCount() const
{
	return referenceCount;
}

NodePtr Context::GetRoot()
{
	return NodePtr(root);
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
