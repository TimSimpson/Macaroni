#ifndef MACARONI_MODEL_CPP_CLASS_LH
#define MACARONI_MODEL_CPP_CLASS_LH

#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Class;
typedef boost::intrusive_ptr<Class> ClassPtr;

void intrusive_ptr_add_ref(Class * p);
void intrusive_ptr_release(Class * p);

END_NAMESPACE

#endif
