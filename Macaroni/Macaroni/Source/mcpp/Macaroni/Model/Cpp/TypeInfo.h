#ifndef MACARONI_MODEL_CPP_TYPEINFO_H
#define MACARONI_MODEL_CPP_TYPEINFO_H

#include "../../ME.h"
#include "../Member.h"
#include "../NodePtr.h"
#include <sstream>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Information on a variables type.  Has the Type itself, which is something
 * existing in a Node, as well as the modifiers for pointers and references. */
struct TypeInfo
{
	bool IsConst;
	bool IsConstPointer;
	bool IsPointer;
	bool IsReference;
	Model::NodePtr Node;

	TypeInfo();

	bool operator==(const TypeInfo & other) const;
	
	void DescribeDifferences(const TypeInfo & info, std::stringstream & stream) const;
};

END_NAMESPACE

#endif
