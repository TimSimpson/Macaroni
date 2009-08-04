#ifndef MACARONI_AXIOM_H
#define MACARONI_AXIOM_H

#include "../ME.h"
#include "Axiom.lh"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents some rule or principal which client code uses to create or alter
 * the Model. */
class Axiom
{
friend void intrusive_ptr_add_ref(Axiom * p);
friend void intrusive_ptr_release(Axiom * p);
public:
	virtual const std::string ToString() const = 0;
protected:
	Axiom(){}
	virtual ~Axiom(){}
	
	/** Called when dereferenced by a smart pointer.  Smart pointer will delete
	 * the object if false is returned. */
	virtual bool onDereference() = 0;

	/** Called when referenced by a smart pointer. */
	virtual void onReference() = 0;

};

END_NAMESPACE2

#endif
