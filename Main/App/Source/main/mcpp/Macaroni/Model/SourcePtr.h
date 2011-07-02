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
#ifndef MACARONI_SOURCE_LH
#define MACARONI_SOURCE_LH

#include "../ME.h"
#include <boost/intrusive_ptr.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

class Source;
typedef boost::intrusive_ptr<Source> SourcePtr;

void intrusive_ptr_add_ref(Source * p);
void intrusive_ptr_release(Source * p);

END_NAMESPACE2

#endif
