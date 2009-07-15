#ifndef MACARONI_MODEL_CPP_TYPE_H
#define MACARONI_MODEL_CPP_TYPE_H

#include "../../ME.h"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "Type.lh"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a type. A type points to a Node in the Model namespace but also
 * contains information on C / C++ type modifiers. */
class Type : public ScopeMember
{
friend void intrusive_ptr_add_ref(Type * p);
friend void intrusive_ptr_release(Type * p);

public:

	static TypePtr Create(NodePtr home, const VariableTypeInfo & typeInfo, Model::ReasonPtr reason);

	virtual ~Type();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

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

private:
	
	Type(Node * home, Model::ReasonPtr reason, const VariableTypeInfo & typeInfo);

	VariableTypeInfo typeInfo;
};

END_NAMESPACE

#endif
