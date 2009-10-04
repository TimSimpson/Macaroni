#ifndef MACARONI_MODEL_TYPEPTR_H
#define MACARONI_MODEL_TYPEPTR_H

#include "../ME.h"
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Type;
typedef boost::shared_ptr<Type> TypePtr;

typedef std::vector<TypePtr> TypeList;
typedef boost::shared_ptr<TypeList> TypeListPtr;

END_NAMESPACE2

#endif
