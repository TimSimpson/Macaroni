#ifndef MACARONI_MODEL_CPP_NAMESPACE_H
#define MACARONI_MODEL_CPP_NAMESPACE_H

#include "../../ME.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Member.h"
#include "Namespace.lh"
#include "Scope.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Namespace : public Scope
{
friend struct NamespaceLuaFunctions;
friend void intrusive_ptr_add_ref(Namespace * p);
friend void intrusive_ptr_release(Namespace * p);
public:	
	Namespace(Node * home, ReasonPtr reason);

	//ClassPtr FindClass(std::string & name);

	//NamespacePtr GetParent() const;

	/** Gets the root at the core of this Namespace tree. */
	//NamespacePtr GetRoot();

	//ClassPtr InsertClass(ClassPtr newInstance);

	//static NamespacePtr GetRoot();

	virtual ~Namespace();

	virtual const char * GetTypeName() const;

protected:	

private:
	
	/** Assumes the given namespace does not exist and creates it as a child
	 * of this namespace. */
	//Namespace * create(std::string & name);

	//Namespace * find(std::string & name);
};

void intrusive_ptr_add_ref(Namespace * p);

void intrusive_ptr_release(Namespace * p);

END_NAMESPACE

#endif