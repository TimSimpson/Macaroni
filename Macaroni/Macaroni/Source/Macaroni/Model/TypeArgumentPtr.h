#ifndef MACARONI_MODEL_TYPEARGUMENT_LH
#define MACARONI_MODEL_TYPEARGUMENT_LH

#include "../ME.h"
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

class TypeArgument;
typedef boost::shared_ptr<TypeArgument> TypeArgumentPtr;

typedef std::vector<TypeArgumentPtr> TypeArgumentList;
typedef boost::shared_ptr<TypeArgumentList> TypeArgumentListPtr;

END_NAMESPACE2

#endif
