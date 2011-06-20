#ifndef MACARONI_AXIOM_CPP
#define MACARONI_AXIOM_CPP

#include "Axiom.h"

BEGIN_NAMESPACE2(Macaroni, Model)

bool Axiom::operator== (const Axiom & other) const
{
	return (this->ToString() == other.ToString());
}

void intrusive_ptr_add_ref(Axiom * p)
{
	p->onReference();
}

void intrusive_ptr_release(Axiom * p)
{
	if (!(p->onDereference()))
	{
		delete p;
	}
}

END_NAMESPACE2

#endif


