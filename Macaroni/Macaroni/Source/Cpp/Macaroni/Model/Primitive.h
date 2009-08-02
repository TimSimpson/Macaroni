#ifndef MACARONI_MODEL_PRIMITIVE_H
#define MACARONI_MODEL_PRIMITIVE_H

#include "../ME.h"
#include "Primitive.lh"
#include "ScopeMember.h"
#include "Type.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Baseclass of Class and Primitive types. */
class Primitive : public ScopeMember, public Type
{
friend void intrusive_ptr_add_ref(Primitive * p);
friend void intrusive_ptr_release(Primitive * p);

public:
	Primitive(Node * scope, std::string & name);
	
	~Primitive();
	
	const std::string & GetTypeName() const;
	
	NodePtr GetTypeNode() const;
private:
};

void intrusive_ptr_add_ref(Primitive * p);

void intrusive_ptr_release(Primitive * p);

END_NAMESPACE2

#endif

