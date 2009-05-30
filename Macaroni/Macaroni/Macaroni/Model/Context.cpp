#ifndef MACARONI_MODEL_CONTEXT_CPP
#define MACARONI_MODEL_CONTEXT_CPP

#include "Context.h"
#include "Namespace.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Context::Context(const std::string & rootName, const std::string & wildcardName)
:referenceCount(0), root(nullptr), wildcard(nullptr)
{
	root = new Namespace(nullptr, rootName);
	root->context = this;
	wildcard = new Namespace(nullptr, wildcardName);
	wildcard->context = this;
}
	
Context::~Context()
{
	MACARONI_ASSERT(referenceCount == 0, "Ref count != 0 on Context destruction!");
	delete root;
	delete wildcard;	
}

ContextPtr Context::Create(const std::string & rootName, const std::string & wildcardName)
{
	return ContextPtr(new Context(rootName, wildcardName));
}

ScopeMember * Context::findMember(const Node * scope, const std::string & complexName)
{
	MACARONI_ASSERT(scope != nullptr, "Cannot handle nullptr here.");	
	std::string firstPart, lastPart;
	Node::SplitFirstNameOffComplexName(complexName, firstPart, lastPart);
	ScopeMember * member = scope->find(firstPart);
	if (member == nullptr)
	{
		return nullptr;
	}
	if (lastPart.empty())
	{
		return member;
	}
	if (dynamic_cast<Node *>(member))
	{
		return findMember(dynamic_cast<Node *>(member), lastPart);
	}
	else
	{
		return nullptr;
	}
}

int Context::GetReferenceCount() const
{
	return referenceCount;
}

/** The root namespace could be considered the global namespace. */
NamespacePtr Context::GetRootNamespace()
{
	return NamespacePtr(root);
}

/** The wildcard namespace matches anything. It is a good spot to put
 * primitive types. */
NamespacePtr Context::GetWildcardNamespace()
{
	return NamespacePtr(wildcard);
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
