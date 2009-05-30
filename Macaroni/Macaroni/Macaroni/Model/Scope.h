#ifndef MACARONI_MODEL_SCOPE_H
#define MACARONI_MODEL_SCOPE_H

#include "../ME.h"
#include <vector>
#include "Context.lh"
#include "Node.lh"
#include "ScopeMember.h"
#include "UnknownScope.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Scope : ScopeMember
{
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:		
	size_t GetMemberCount() const;

	ScopeMemberPtr GetMember(int index) const;

protected:
	Scope(Node * scope);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	~Scope();

private:
};

END_NAMESPACE2

#endif
