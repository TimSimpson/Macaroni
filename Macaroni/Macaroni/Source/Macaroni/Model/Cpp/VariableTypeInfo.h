#ifndef MACARONI_MODEL_CPP_VARIABLETYPEINFO_H
#define MACARONI_MODEL_CPP_VARIABLETYPEINFO_H

#include "../../ME.h"
#include "../Member.h"
#include "../Node.lh"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Information on a variables type.  Has the Type itself, which is something
 * existing in a Node, as well as the modifiers for pointers and references. */
struct VariableTypeInfo
{
	bool IsConst;
	bool IsConstPointer;
	bool IsPointer;
	bool IsReference;
	Model::NodePtr Type;

	VariableTypeInfo()
		:IsConst(false),
		 IsConstPointer(false),
		 IsPointer(false),
		 IsReference(false),
		 Type()
	{
	}

	bool operator==(const VariableTypeInfo & other) const 
	{
		return IsConst == other.IsConst && IsConstPointer == other.IsConstPointer
			&& IsPointer == other.IsPointer && IsReference == other.IsReference
			&& Type == other.Type;
	}
};

END_NAMESPACE

#endif
