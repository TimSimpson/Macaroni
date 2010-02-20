#ifndef MACARONI_MODEL_LIBRARY_CPP
#define MACARONI_MODEL_LIBRARY_CPP

#include "../ME.h"
#include "Context.h"
#include "Library.h"

BEGIN_NAMESPACE2(Macaroni, Model)


Library::Library(Context * context, const std::string & name, const std::string & version)
:context(context),
 name(name),
 referenceCount(0),
 version(version)
{
}

Library::~Library()
{
	int five = 5;
}

void intrusive_ptr_add_ref(Library * p)
{
	intrusive_ptr_add_ref(p->context);
}

void intrusive_ptr_release(Library * p)
{
	intrusive_ptr_release(p->context);	
}

END_NAMESPACE2

#endif
