#ifndef MACARONI_MODEL_NODE_LH
#define MACARONI_MODEL_NODE_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

class Node;
typedef boost::intrusive_ptr<Node> NodePtr;

void intrusive_ptr_add_ref(Node * p);
void intrusive_ptr_release(Node * p);

typedef std::vector<NodePtr> NodeList;
typedef boost::shared_ptr<NodeList> NodeListPtr;

END_NAMESPACE2

#endif
