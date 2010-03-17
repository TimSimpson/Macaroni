#ifndef MACARONI_MODEL_ATTRIBUTETABLEPTR_H
#define MACARONI_MODEL_ATTRIBUTETABLEPTR_H

#include <boost/shared_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class AttributeTable;

typedef boost::shared_ptr<AttributeTable> AttributeTablePtr;

END_NAMESPACE2

#endif

