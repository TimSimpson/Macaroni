#ifndef MACARONI_MODEL_CPP_DESTRUCTORPTR_H
#define MACARONI_MODEL_CPP_DESTRUCTORPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Destructor;
typedef boost::intrusive_ptr<Destructor> DestructorPtr;

END_NAMESPACE

#endif
