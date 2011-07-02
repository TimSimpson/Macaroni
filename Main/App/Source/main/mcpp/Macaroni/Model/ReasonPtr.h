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
#ifndef MACARONI_REASON_LH
#define MACARONI_REASON_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Reason;
typedef boost::intrusive_ptr<Reason> ReasonPtr;

void intrusive_ptr_add_ref(Reason * p);
void intrusive_ptr_release(Reason * p);

END_NAMESPACE2

#endif
