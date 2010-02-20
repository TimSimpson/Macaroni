#ifndef MACARONI_SOURCE_LH
#define MACARONI_SOURCE_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Source;
typedef boost::intrusive_ptr<Source> SourcePtr;

void intrusive_ptr_add_ref(Source * p);
void intrusive_ptr_release(Source * p);

END_NAMESPACE2

#endif
