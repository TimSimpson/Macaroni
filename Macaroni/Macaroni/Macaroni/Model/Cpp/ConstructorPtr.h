#ifndef MACARONI_MODEL_CPP_CONSTRUCTORPTR_H
#define MACARONI_MODEL_CPP_CONSTRUCTORPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Constructor;
typedef boost::intrusive_ptr<Constructor> ConstructorPtr;

END_NAMESPACE

#endif
