#ifndef MACARONI_MODEL_CPP_CPPCONTEXT_H
#define MACARONI_MODEL_CPP_CPPCONTEXT_H

#include "../../ME.h"
#include "../Context.h"
#include "CppContextPtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** This class has static methods for creating a Context with C++ specific nodes
 * such as Primitives. */
class CppContext
{
public:	
	/** Adds the C++ goodness to an existing context. */
	static void CreateCppNodes(ContextPtr & context);

	/** Returns a secret node which stores primitives. */
	static NodePtr GetPrimitives(ContextPtr & context);

private:
	static void addPrimitive(NodePtr & primitiveNode, const char * name);
}; 

END_NAMESPACE

#endif
