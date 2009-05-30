#ifndef MACARONI_MODEL_CONTEXT_H
#define MACARONI_MODEL_CONTEXT_H

#include "../ME.h"
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include "Class.lh"
#include "Context.lh"
#include "Namespace.lh"
#include "Node.lh"
#include "ScopeMember.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Context
{
friend void intrusive_ptr_add_ref(Context * p);
friend void intrusive_ptr_release(Context * p);
public:
	Context(const std::string & rootName, const std::string & wildcardName);
	
	~Context();

	static ContextPtr Create(const std::string & rootName, const std::string & wildcardName);

	/** Searches from the root scope. */
	ScopeMemberPtr FindMember(const std::string & complexName);

	/** Searches from the specified scope for a given name. */
	ScopeMemberPtr FindMember(const NodePtr scope, const std::string & complexName);

	int GetReferenceCount() const;

	/** The root namespace could be considered the global namespace. */
	NamespacePtr GetRootNamespace();

	/** The wildcard namespace matches anything. It is a good spot to put
	 * primitive types. */
	NamespacePtr GetWildcardNamespace();	

private:

	ScopeMember * findMember(const Node * scope, const std::string & complexName);

	void onAddReference();
	
	void onReleaseReference();

	int referenceCount;
	
	Namespace * root;

	Namespace * wildcard;
};

inline void intrusive_ptr_add_ref(Context * p)
{
	p->onAddReference();
}

inline void intrusive_ptr_release(Context * p)
{
	p->onReleaseReference();
}



END_NAMESPACE2

#endif
