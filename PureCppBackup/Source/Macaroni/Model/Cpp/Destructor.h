#ifndef MACARONI_MODEL_CPP_DESTRUCTOR_H
#define MACARONI_MODEL_CPP_DESTRUCTOR_H

#include "../../ME.h"
#include "DestructorPtr.h"
#include "Function.h"
#include "../Member.h"
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "TypeInfo.h"
#include "VariablePtr.h"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Destructor : public Function
{
friend void intrusive_ptr_add_ref(Destructor * p);
friend void intrusive_ptr_release(Destructor * p);

public:

	static DestructorPtr Create(NodePtr home, bool isInline, Access access, Model::ReasonPtr reason);

	virtual ~Destructor();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Destructor(Node * home, Model::ReasonPtr reason, bool isInline, Access access);
	
};

END_NAMESPACE

#endif