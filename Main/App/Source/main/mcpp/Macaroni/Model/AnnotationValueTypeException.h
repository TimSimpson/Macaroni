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
#ifndef MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H
#define MACARONI_MODEL_ATTRIBUTEVALUETYPEEXCEPTION_H

#include <Macaroni/ME.h>
#include <Macaroni/Exception.h>

BEGIN_NAMESPACE2(Macaroni, Model)

class AnnotationValueTypeException : public Macaroni::Exception 
{
public:
	AnnotationValueTypeException(std::string message)
		: Macaroni::Exception(message.c_str()) {}
};

END_NAMESPACE2

#endif
