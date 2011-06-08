#ifndef MACARONI_MODEL_CPP_PRIMITIVE_H
#define MACARONI_MODEL_CPP_PRIMITIVE_H

#include "../../ME.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Member.h"
#include "PrimitivePtr.h"
#include "ScopeMember.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Primitive : public ScopeMember
{
friend void intrusive_ptr_add_ref(Primitive * p);
friend void intrusive_ptr_release(Primitive * p);
public:	
	static PrimitivePtr Create(NodePtr home, ReasonPtr reason);

	virtual ~Primitive();

	//void AddCppPrimitives(ContextPtr context);

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	virtual void Visit(MemberVisitor * visitor) const;

protected:	
	Primitive(Node * home, ReasonPtr reason);
private:
	
};

void intrusive_ptr_add_ref(Primitive * p);

void intrusive_ptr_release(Primitive * p);

END_NAMESPACE

#endif
