#ifndef MACARONI_MODEL_SCOPEMEMBER_H
#define MACARONI_MODEL_SCOPEMEMBER_H

#include "../ME.h"
#include "Context.lh"
#include <string>
#include "Scope.lh"
#include "ScopeMember.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

/** All classes within Macaroni are objects that exist in a Scope and are
 * therefore Scope members. */
class ScopeMember
{
friend void intrusive_ptr_add_ref(ScopeMember * p);
friend void intrusive_ptr_release(ScopeMember * p);
public:
	const std::string & GetName() const;

	std::string GetFullName() const;

	ScopePtr GetScope() const;
	
	virtual ~ScopeMember();
protected:
	ScopeMember(Scope * scope, const std::string & name);
	ScopeMember(const ScopeMember & other);
	void operator=(const ScopeMember & other);	

	Scope * getScope() const;

private:
	std::string name;

	Scope * scope;
};

void intrusive_ptr_add_ref(ScopeMember * p);

void intrusive_ptr_release(ScopeMember * p);

END_NAMESPACE2

#endif
