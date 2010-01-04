#ifndef MACARONI_MODEL_CPP_VARIABLE_H
#define MACARONI_MODEL_CPP_VARIABLE_H

#include "../../ME.h"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "Variable.lh"
#include "../Type.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a variable.  Can be in a namespace, in a class, or in a function
 * (the argument list). */
class Variable : public ScopeMember
{
friend void intrusive_ptr_add_ref(Variable * p);
friend void intrusive_ptr_release(Variable * p);

public:

	static VariablePtr Create(NodePtr home, Access access, bool isStatic, const TypePtr type, Model::ReasonPtr reason);

	virtual ~Variable();

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	virtual const char * GetTypeName() const;

	inline const TypePtr GetType() const
	{
		return type;
	}

	//Model::NodePtr GetTypeNode() const;

	////TODO: Remove these accessors, they're obnoxiously redundant.
	//inline bool IsConst()
	//{
	//	return type->IsConst;
	//}

	//inline bool IsConstPointer()
	//{
	//	return type->IsConstPointer;
	//}

	//inline bool IsPointer()
	//{
	//	return type->IsPointer;
	//}

	//inline bool IsReference()
	//{
	//	return type->IsReference;
	//}

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Variable(Node * home, Model::ReasonPtr reason, Access access, bool isStatic, const TypePtr type);

	TypePtr type;
};

END_NAMESPACE

#endif
