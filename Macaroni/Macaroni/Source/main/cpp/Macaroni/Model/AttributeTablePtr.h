#ifndef MACARONI_MODEL_ATTRIBUTETABLEPTR_H
#define MACARONI_MODEL_ATTRIBUTETABLEPTR_H

#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeTable;

typedef boost::shared_ptr<AttributeTable> AttributeTableInternalPtr;
typedef boost::intrusive_ptr<AttributeTable> AttributeTablePtr;

void intrusive_ptr_add_ref(AttributeTable * p);
void intrusive_ptr_release(AttributeTable * p);

END_NAMESPACE2

#endif

