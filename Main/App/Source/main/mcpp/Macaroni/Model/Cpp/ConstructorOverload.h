#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_H
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOAD_H

#include "../../ME.h"
#include "Constructor.h"
#include "ConstructorOverloadPtr.h"
#include "Function.h"
#include "FunctionOverload.h"
#include "../Member.h"
#include <Macaroni/Model/Node.h>
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "TypeInfo.h"
#include "VariablePtr.h"
#include "VariableAssignment.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class ConstructorOverload : public FunctionOverload
{
friend void intrusive_ptr_add_ref(ConstructorOverload * p);
friend void intrusive_ptr_release(ConstructorOverload * p);

public:

	ConstructorPtr ConstructorOverload::Create(NodePtr host, bool isInline, 
											   AccessPtr access, 
											   Model::ReasonPtr reason);

	static ConstructorOverloadPtr Create(ConstructorPtr home, 
		bool isInline, AccessPtr access, Model::ReasonPtr reason);

	virtual ~ConstructorOverload();

	void AddAssignment(const VariableAssignment & assignment);

	virtual bool canBeChildOf(const Member * other) const;

	ConstructorOverloadPtr Create(FunctionPtr home, bool isInline, const Access access, 
							   const bool isStatic, const TypePtr rtnType, 
							   bool constMember, Model::ReasonPtr reason);

	const VariableAssignment & GetAssignment(int index) const;

	int GetAssignmentCount() const;
	
	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	ConstructorOverload(Model::Node * home, Model::ReasonPtr reason, bool isInline, Access access);
	
	std::vector<VariableAssignment> assignments;	
};

END_NAMESPACE

#endif
