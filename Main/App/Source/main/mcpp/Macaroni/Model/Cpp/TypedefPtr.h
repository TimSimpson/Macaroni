#ifndef MACARONI_MODEL_CPP_TYPEDEFPTR_H
#define MACARONI_MODEL_CPP_TYPEDEFPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Typedef;
typedef boost::intrusive_ptr<Typedef> TypedefPtr;

void intrusive_ptr_add_ref(Typedef * p);
void intrusive_ptr_release(Typedef * p);

END_NAMESPACE

#endif
