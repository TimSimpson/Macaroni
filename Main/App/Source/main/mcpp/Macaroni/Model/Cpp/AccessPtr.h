#ifndef MACARONI_MODEL_CPP_ACCESSPTR_H
#define MACARONI_MODEL_CPP_ACCESSPTR_H

#include "../../ME.h"
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Access;

typedef boost::shared_ptr<Access> AccessPtr;

END_NAMESPACE

#endif
