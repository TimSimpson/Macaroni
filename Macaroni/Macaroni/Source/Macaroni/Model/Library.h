#ifndef MACARONI_MODEL_LIBRARY_H
#define MACARONI_MODEL_LIBRARY_H

#include "../ME.h"
#include "Context.h"
#include "FileNamePtr.h"
#include "Library.lh"
#include "Member.lh"
#include "Node.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Nodes always belong to a library.  They coexist next to each other in the 
 * Model tree, but they all keep a reference to the library they were created
 * under. */
class Library
{ 
friend Context;
friend void intrusive_ptr_add_ref(Library * p);
friend void intrusive_ptr_release(Library * p);

public:			

	inline ContextPtr GetContext() const
	{
		return ContextPtr(context);
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	inline const std::string & GetVersion() const
	{
		return version;
	}

private:
	Library(Context * context, const std::string & name, const std::string & version);

	Context * context;
	int referenceCount;
	std::string name;
	std::string version;
};

END_NAMESPACE2

#endif
