#ifndef MACARONI_MODEL_CPP_SCOPEMEMBER_H
#define MACARONI_MODEL_CPP_SCOPEMEMBER_H

#include "../../ME.h"
#include <string>
#include "../Member.h"
#include "ScopeMember.lh"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Concreate data concerning a Node.  In Macaroni, all nodes exist in a Scope
 * and are therefore ScopeMembers.  This class provides a link back to a node
 * which it uses to get certain useful data such as the name.  Methods for the
 * creation of new Model data remains in the Node. */
class ScopeMember : public Model::Member
{
friend void intrusive_ptr_add_ref(ScopeMember * p);
friend void intrusive_ptr_release(ScopeMember * p);
public:
	virtual ~ScopeMember();

	const std::string & GetName() const;

	std::string GetFullName() const;	
		
protected:
	ScopeMember(Model::Node * node, const char * typeName, ReasonPtr reason);

	ScopeMember(const ScopeMember & other);

	void operator=(const ScopeMember & other);	
};

END_NAMESPACE

#endif
