#ifndef MACARONI_MODEL_TYPE_H
#define MACARONI_MODEL_TYPE_H

#include "../ME.h"
#include "Node.h"
#include "TypePtr.h"
#include <Macaroni/Model/TypeModifiers.h>
#include "TypeArgumentPtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)


//TODO: This HAS to start living in the Context.  It is a memory leak... can't
// believe I didn't notice it at the time.
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
	
	bool operator== (const Type & other) const;

	void DescribeDifferences(const TypePtr info, std::stringstream & stream) const;

	NodePtr GetNode() const;

	TypeArgumentListPtr GetTypeArguments() const;
	
	inline bool IsConst() const
	{
		return isConst;
	}

	bool IsConstPointer() const
	{
		return isConstPointer;
	}

	/** Light means that the type is a "light dependency", which means it can
	 * be satisfied with a forward reference. The '~light' keyword is used
	 * for this. It useful with complex template types Macaroni isn't smart 
	 * enough to understand which end up just needing forward references. */
	bool IsLight() const
	{
		return isLight;
	}

	bool IsPointer() const
	{
		return isPointer;
	}

	bool IsReference() const
	{
		return isReference;
	}

	static bool ListContains(TypeListPtr list, TypePtr target);

	static bool ListIsEqual(TypeListPtr list1, TypeListPtr list2);

private:
	bool isConst;
	bool isConstPointer;
	bool isLight;
	bool isPointer;
	bool isReference;

	NodePtr type;	
	TypeArgumentListPtr typeArguments;
};

END_NAMESPACE2

#endif
