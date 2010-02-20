#ifndef MACARONI_MODEL_BLOCKPTR_H
#define MACARONI_MODEL_BLOCKPTR_H

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Block;
typedef boost::intrusive_ptr<Block> BlockPtr;

void intrusive_ptr_add_ref(Block * p);
void intrusive_ptr_release(Block * p);

END_NAMESPACE2

#endif
