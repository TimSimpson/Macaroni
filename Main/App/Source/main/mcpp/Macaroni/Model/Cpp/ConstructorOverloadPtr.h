#ifndef MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADPTR_H
#define MACARONI_MODEL_CPP_CONSTRUCTOROVERLOADPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class ConstructorOverload;
typedef boost::intrusive_ptr<ConstructorOverload> ConstructorOverloadPtr;

END_NAMESPACE

#endif
