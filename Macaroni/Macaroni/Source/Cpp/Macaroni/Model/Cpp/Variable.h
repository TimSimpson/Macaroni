#ifndef MACARONI_MODEL_CPP_VARIABLE_H
#define MACARONI_MODEL_CPP_VARIABLE_H

#include "../../ME.h"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "Variable.lh"
#include "TypeInfo.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a variable.  Can be in a namespace, in a class, or in a function
 * (the argument list). */
class Variable : public ScopeMember
{
friend void intrusive_ptr_add_ref(Variable * p);
friend void intrusive_ptr_release(Variable * p);

public:

	static VariablePtr Create(NodePtr home, const TypeInfo & typeInfo, Model::ReasonPtr reason);

	virtual ~Variable();

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	virtual const char * GetTypeName() const;

	inline const TypeInfo & GetTypeInfo() const
	{
		return typeInfo;
	}

	Model::NodePtr GetTypeNode() const;

	inline bool IsConst()
	{
		return typeInfo.IsConst;
	}

	inline bool IsConstPointer()
	{
		return typeInfo.IsConstPointer;
	}

	inline bool IsPointer()
	{
		return typeInfo.IsPointer;
	}

	inline bool IsReference()
	{
		return typeInfo.IsReference;
	}

	virtual void Visit(MemberVisitor * visitor) const;

private:
	
	Variable(Node * home, Model::ReasonPtr reason, const TypeInfo & typeInfo);

	TypeInfo typeInfo;
};

END_NAMESPACE

#endif
