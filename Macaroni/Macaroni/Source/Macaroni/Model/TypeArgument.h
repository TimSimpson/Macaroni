#ifndef MACARONI_MODEL_TYPEARGUMENT_H
#define MACARONI_MODEL_TYPEARGUMENT_H

#include "../ME.h"
#include "Node.h"
#include "TypeArgumentPtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Connects a type to a NodeList of arguments for that type. */
class TypeArgument 
{
public:
	TypeArgument(NodePtr type);
	TypeArgument(NodePtr type, NodeListPtr typeArguments);
	
	~TypeArgument();
	
	NodeListPtr GetArguments();

	NodePtr GetNode();

private:
	NodeListPtr arguments;
	NodePtr node;	
};

END_NAMESPACE2

#endif
