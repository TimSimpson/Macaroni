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
#ifndef MACARONI_MODEL_ATTRIBUTEVALUEPTR_H
#define MACARONI_MODEL_ATTRIBUTEVALUEPTR_H

#include "../ME.h"
#include "AnnotationValue.h"
#include <boost/intrusive_ptr.hpp>
#include <boost/shared_ptr.hpp>


BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationValue;

typedef boost::shared_ptr<AnnotationValue> AnnotationValueInternalPtr;
typedef boost::intrusive_ptr<AnnotationValue> AnnotationValuePtr;
void intrusive_ptr_add_ref(AnnotationValue * p);
void intrusive_ptr_release(AnnotationValue * p);

END_NAMESPACE2

#endif

