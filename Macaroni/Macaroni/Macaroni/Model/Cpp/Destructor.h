#ifndef MACARONI_MODEL_CPP_DESTRUCTOR_H
#define MACARONI_MODEL_CPP_DESTRUCTOR_H

#include "../../ME.h"
#include "DestructorPtr.h"
#include "Function.h"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "../Source.lh"
#include "TypeInfo.h"
#include "Variable.lh"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Destructor : public Function
{
friend void intrusive_ptr_add_ref(Destructor * p);
friend void intrusive_ptr_release(Destructor * p);

public:

	static DestructorPtr Create(NodePtr home, Model::ReasonPtr reason);

	virtual ~Destructor();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Destructor(Node * home, Model::ReasonPtr reason);
	
};

END_NAMESPACE

#endif
