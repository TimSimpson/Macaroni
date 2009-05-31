#ifndef MACARONI_MODEL_SCOPEMEMBER_H
#define MACARONI_MODEL_SCOPEMEMBER_H

#include "../ME.h"
#include "Context.lh"
#include <string>
#include "Node.lh"
#include "ScopeMember.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Concreate data concerning a Node.  In Macaroni, all nodes exist in a Scope
 * and are therefore ScopeMembers.  This class provides a link back to a node
 * which it uses to get certain useful data such as the name. */
class ScopeMember
{
friend void intrusive_ptr_add_ref(ScopeMember * p);
friend void intrusive_ptr_release(ScopeMember * p);
public:
	const std::string & GetName() const;

	std::string GetFullName() const;

	NodePtr GetNode() const;
	
	virtual ~ScopeMember();
protected:
	ScopeMember(Node * node);

	ScopeMember(const ScopeMember & other);

	void operator=(const ScopeMember & other);	

	Node * getNode() const;

	Node * node;
};

void intrusive_ptr_add_ref(ScopeMember * p);

void intrusive_ptr_release(ScopeMember * p);

END_NAMESPACE2

#endif
