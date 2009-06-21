#ifndef MACARONI_MODEL_CPP_SCOPE_H
#define MACARONI_MODEL_CPP_SCOPE_H

#include "../../ME.h"
#include <vector>
#include "../Member.h"
#include "../Node.lh"
#include "ScopeMember.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Scope : public ScopeMember
{
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:		
	size_t GetMemberCount() const;

	NodePtr GetMember(int index) const;

	virtual const char * GetTypeName() const = 0;

protected:
	Scope(Node * scope, ReasonPtr reason);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	virtual ~Scope(){}

private:
};

END_NAMESPACE

#endif
