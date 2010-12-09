#ifndef MACARONI_MODEL_CPP_SCOPE_H
#define MACARONI_MODEL_CPP_SCOPE_H

#include "../../ME.h"
#include <vector>
#include "../LibraryMember.h"
#include "../Member.h"
#include "../NodePtr.h"
#include "ScopeMember.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Scope : public ScopeMember, public virtual Model::LibraryMember
{
friend void intrusive_ptr_add_ref(Scope * p);
friend void intrusive_ptr_release(Scope * p);

public:		
	virtual Model::LibraryPtr GetLibrary() const;

	size_t GetMemberCount() const;

	NodePtr GetMember(int index) const;

	virtual const char * GetTypeName() const = 0;

protected:
	Scope(Library * library, Node * scope, const char * typeName, ReasonPtr reason);
	Scope(const Scope & other);
	void operator=(const Scope & other);
	virtual ~Scope(){}

private:
	Model::Library * library;
};

END_NAMESPACE

#endif
