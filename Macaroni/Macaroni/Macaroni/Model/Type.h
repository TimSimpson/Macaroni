#ifndef MACARONI_MODEL_TYPE_H
#define MACARONI_MODEL_TYPE_H

#include "../ME.h"
#include "Node.lh"
#include "Type.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Type
{
public:
friend void intrusive_ptr_add_ref(Type * p);
friend void intrusive_ptr_release(Type * p);

	Type(){}
	/** It really pisses me off, but there's no way for Type to inhereit from 
	 * ScopeMember without preventing Class from subclassing Node.  So Type has
	 * to be abstract with virtual functions mirroring those from Node. */
	virtual const std::string & GetTypeName() const = 0;
	virtual NodePtr GetTypeNode() const = 0;
protected:
	~Type(){}
private:
	/** The best guess header file. */
	//std::string & headerFile;
	/** The Macaroni file which defines it. */
	//std::string & macaroniFile;
};

void intrusive_ptr_add_ref(Type * p);

void intrusive_ptr_release(Type * p);

END_NAMESPACE2

#endif

