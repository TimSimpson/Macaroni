#ifndef MACARONI_MODEL_CPP_CONSTRUCTOR_H
#define MACARONI_MODEL_CPP_CONSTRUCTOR_H

#include "../../ME.h"
#include "ConstructorPtr.h"
#include "Function.h"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "../Source.lh"
#include "TypeInfo.h"
#include "Variable.lh"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Constructor : public Function
{
friend void intrusive_ptr_add_ref(Constructor * p);
friend void intrusive_ptr_release(Constructor * p);

public:

	static ConstructorPtr Create(NodePtr home, Model::ReasonPtr reason);

	virtual ~Constructor();

	void AddAssignment(const VariableAssignment & assignment);

	virtual bool canBeChildOf(const Member * other) const;

	const VariableAssignment & GetAssignment(int index) const;

	int GetAssignmentCount() const;

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Constructor(Node * home, Model::ReasonPtr reason);
	
	std::vector<VariableAssignment> assignments;	
};

END_NAMESPACE

#endif
