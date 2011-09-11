#ifndef MACARONI_MODEL_LIBRARYMEMBER_H
#define MACARONI_MODEL_LIBRARYMEMBER_H

#include "../ME.h"
#include "Library.h"


BEGIN_NAMESPACE2(Macaroni, Model)

/** A trait which defines a Member to be "owned" by a Library. */
class LibraryElement
{ 
public:		
	virtual LibraryPtr GetLibrary() const = 0;
};

END_NAMESPACE2

#endif
