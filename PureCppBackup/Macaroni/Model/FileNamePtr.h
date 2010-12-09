#ifndef MACARONI_FILENAME_LH
#define MACARONI_FILENAME_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class FileName;
typedef boost::intrusive_ptr<FileName> FileNamePtr;

void intrusive_ptr_add_ref(FileName * p);
void intrusive_ptr_release(FileName * p);

END_NAMESPACE2

#endif
