#ifndef MACARONI_MODEL_TYPEARGUMENT_H
#define MACARONI_MODEL_TYPEARGUMENT_H

#include "../ME.h"
#include "Node.h"
#include "Type.h"
#include "TypeArgumentPtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Connects a type to a NodeList of arguments for that type. */

// TODO: Why did I make this *and* the type class? Couldn't type have just
// had an argumentList of itself? I am completely dumbfounded by the realization 
// they are the same.

class TypeArgument 
{
public:
	TypeArgument(NodePtr type);
	TypeArgument(NodePtr type, TypeListPtr typeArguments);
	
	~TypeArgument();
	
	bool operator==(const TypeArgument & other) const;

	TypeListPtr GetArguments() const;

	NodePtr GetNode() const;

	static bool ListContains(TypeArgumentListPtr list, TypeArgumentPtr target);

	static bool ListIsEqual(TypeArgumentListPtr list1, TypeArgumentListPtr list2);

private:
	TypeListPtr arguments;
	NodePtr node;	
};

END_NAMESPACE2

#endif
