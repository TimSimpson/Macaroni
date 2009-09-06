#ifndef MACARONI_ACCESS_H
#define MACARONI_ACCESS_H

#include "../ME.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents a level of access. */
enum Access
{
	Access_Private,
	Access_Protected,
	Access_Public
};


END_NAMESPACE2

#endif
