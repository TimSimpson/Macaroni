#ifndef MACARONI_MODEL_ATTRIBUTEVALUEPTR_H
#define MACARONI_MODEL_ATTRIBUTEVALUEPTR_H

#include "../ME.h"
#include "AttributeValue.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>


BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeValue;

typedef boost::shared_ptr<AttributeValue> AttributeValueInternalPtr;
typedef boost::intrusive_ptr<AttributeValue> AttributeValuePtr;
void intrusive_ptr_add_ref(AttributeValue * p);
void intrusive_ptr_release(AttributeValue * p);

END_NAMESPACE2

#endif

