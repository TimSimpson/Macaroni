#ifndef MACARONI_MODEL_ATTRIBUTEDEFINITIONPTR_H
#define MACARONI_MODEL_ATTRIBUTEDEFINITIONPTR_H

#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeDefinition;
typedef boost::intrusive_ptr<AttributeDefinition> AttributeDefinitionPtr;

void intrusive_ptr_add_ref(AttributeDefinition * p);
void intrusive_ptr_release(AttributeDefinition * p);

END_NAMESPACE2

#endif
