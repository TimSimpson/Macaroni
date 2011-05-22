#ifndef MACARONI_MODEL_TYPEPTR_H
#define MACARONI_MODEL_TYPEPTR_H

#include "../ME.h"
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

// TODO: This is a horrible memory leak.  This thing needs to be connected to
// the Context somehow with an intrusive ptr.

class Type;
typedef boost::shared_ptr<Type> TypePtr;

typedef std::vector<TypePtr> TypeList;
typedef boost::shared_ptr<TypeList> TypeListPtr;

END_NAMESPACE2

#endif
