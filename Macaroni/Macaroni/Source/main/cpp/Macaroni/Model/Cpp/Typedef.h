#ifndef MACARONI_MODEL_CPP_TYPEDEF_H
#define MACARONI_MODEL_CPP_TYPEDEF_H

#include "../../ME.h"
#include "../Member.h"
#include "../Node.h"
#include "ScopeMember.h"
#include "../TypePtr.h"
#include "TypedefPtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a Type which is stored in the Node space. */
class Typedef : public ScopeMember
{
friend void intrusive_ptr_add_ref(Typedef * p);
friend void intrusive_ptr_release(Typedef * p);

public:
	static TypedefPtr Create(Macaroni::Model::NodePtr home, 
							 Macaroni::Model::ReasonPtr reason, 
							 TypePtr type);
	
	~Typedef();

	Macaroni::Model::TypePtr GetType() const;

	virtual const char * GetTypeName() const;

	virtual void Visit(Macaroni::Model::MemberVisitor * visitor) const;

protected:
	Typedef(Macaroni::Model::Node *		home, 
			Macaroni::Model::ReasonPtr	reason, 
			Macaroni::Model::TypePtr	type);

	virtual bool canBeChildOf(const Macaroni::Model::Member * other) const;

private:
	Macaroni::Model::TypePtr type;	
};

END_NAMESPACE

#endif
