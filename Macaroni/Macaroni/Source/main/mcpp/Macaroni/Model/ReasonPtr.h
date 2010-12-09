#ifndef MACARONI_REASON_LH
#define MACARONI_REASON_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Reason;
typedef boost::intrusive_ptr<Reason> ReasonPtr;

void intrusive_ptr_add_ref(Reason * p);
void intrusive_ptr_release(Reason * p);

END_NAMESPACE2

#endif
