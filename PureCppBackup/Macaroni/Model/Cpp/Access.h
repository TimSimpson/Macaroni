#ifndef MACARONI_MODEL_CPP_ACCESS_H
#define MACARONI_MODEL_CPP_ACCESS_H

#include "../../ME.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a level of access. */
enum Access
{
	Access_Private,
	Access_Protected,
	Access_Public,
	Access_NotSpecified
};

END_NAMESPACE

#endif
