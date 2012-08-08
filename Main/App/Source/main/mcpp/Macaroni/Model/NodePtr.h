/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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

typedef std::vector<Node *> NakedNodeList;
typedef boost::shared_ptr<NakedNodeList> NakedNodeListPtr;

END_NAMESPACE2

#endif
