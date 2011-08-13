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
#ifndef MACARONI_MODEL_CPP_TYPEDEFPTR_H
#define MACARONI_MODEL_CPP_TYPEDEFPTR_H

#include "../../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Typedef;
typedef boost::intrusive_ptr<Typedef> TypedefPtr;

void intrusive_ptr_add_ref(Typedef * p);
void intrusive_ptr_release(Typedef * p);

END_NAMESPACE

#endif