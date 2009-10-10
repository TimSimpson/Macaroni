#ifndef MACARONI_MODEL_TYPE_H
#define MACARONI_MODEL_TYPE_H

#include "../ME.h"
#include "Node.h"
#include "TypePtr.h"
#include "TypeArgumentPtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

struct TypeModifiers
{
	bool Const;
	bool ConstPointer;
	bool Pointer;
	bool Reference;

	TypeModifiers()
		: Const(false), ConstPointer(false), Pointer(false), Reference(false)
	{
	}
};

/** Represents a Type.  A type does not exist in the Node system, but does 
 * point to a Node. 
 * This may also store a list of TypeArguments, where each element points to
 * a Node that relates to the type node, with additional nodes offering 
 * argument data. */
class Type
{
public:
	Type(NodePtr type, TypeModifiers modifiers);
	Type(NodePtr type, TypeModifiers modifiers, TypeArgumentListPtr typeArguments);
	
	~Type();
	
	NodePtr GetNode();

	TypeArgumentListPtr GetTypeArguments();
	
	inline bool IsConst() const
	{
		return isConst;
	}

	bool IsConstPointer() const
	{
		return isConstPointer;
	}

	bool IsPointer() const
	{
		return isPointer;
	}

	bool IsReference() const
	{
		return isReference;
	}

private:
	bool isConst;
	bool isConstPointer;
	bool isPointer;
	bool isReference;

	NodePtr type;	
	TypeArgumentListPtr typeArguments;
};

END_NAMESPACE2

#endif
