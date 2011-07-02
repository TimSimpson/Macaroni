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
#ifndef MACARONI_MODEL_TYPEPTR_H
#define MACARONI_MODEL_TYPEPTR_H

#include "../ME.h"
#include <boost/shared_ptr.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

// TODO: This is a horrible memory leak.  This thing needs to be connected to
// the Context somehow with an intrusive ptr.

class Type;
typedef boost::shared_ptr<Type> TypePtr;

typedef std::vector<TypePtr> TypeList;
typedef boost::shared_ptr<TypeList> TypeListPtr;

END_NAMESPACE2

#endif
