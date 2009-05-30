#ifndef MACARONI_MODEL_TYPE_CPP
#define MACARONI_MODEL_TYPE_CPP

#include "Type.h"
#include "ScopeMember.h"

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(Type * p)
{
	intrusive_ptr_add_ref((ScopeMember *) p);
}

void intrusive_ptr_release(Type * p)
{
	intrusive_ptr_release((ScopeMember *) p);
}

END_NAMESPACE2

#endif

