#ifndef MACARONI_MODEL_TYPE_H
#define MACARONI_MODEL_TYPE_H

#include "../ME.h"
#include "Node.h"
#include "TypeArgument.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents a Type.  A type does not exist in the Node system, but does 
 * point to a Node. 
 * This may also store a list of TypeArguments, where each element points to
 * a Node that relates to the type node, with additional nodes offering 
 * argument data. */
class Type
{
public:
	Type(NodePtr type);
	Type(NodePtr type, std::vector<TypeArgument> typeArguments);
	
	~Type();
	
private:
	NodePtr type;	
	std::vector<TypeArgument> typeArguments;
};

END_NAMESPACE2

#endif
