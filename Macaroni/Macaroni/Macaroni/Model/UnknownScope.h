#ifndef MACARONI_MODEL_UNKNOWNSCOPE_H
#define MACARONI_MODEL_UNKNOWNSCOPE_H

#include "../ME.h"
#include <vector>
#include "Context.lh"
#include "Node.h"
#include "ScopeMember.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** This Node is a stand-in when the parser needs a place holder for a Node 
 * that could be a Namespace, a Class, or something else.  Initially, it just
 * does typical Nodey things but can be modified after creation to refer
 * to another Node object and chain all messages to the object. */
class UnknownScope : public Node
{
friend void intrusive_ptr_add_ref(UnknownScope * p);
friend void intrusive_ptr_release(UnknownScope * p);

public:	
	UnknownScope(Node * scope, const std::string & name);

	~UnknownScope();

	virtual ScopeMemberPtr Find(const std::string & name);

	virtual size_t GetMemberCount() const;

	ClassPtr IsClass();

	NamespacePtr IsNamespace();

	virtual bool IsRoot() const;

	virtual ScopeMemberPtr GetMember(int index) const;

protected:
	UnknownScope(const Node & other);
	void operator=(const Node & other);

private:
	Node * knownNode;
};

void intrusive_ptr_add_ref(UnknownScope * p);

void intrusive_ptr_release(UnknownScope * p);

END_NAMESPACE2

#endif
