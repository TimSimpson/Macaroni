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
#ifndef MACARONI_PARSER_CPP_CPPAXIOM_H
#define MACARONI_PARSER_CPP_CPPAXIOM_H

#include "../../ME.h"
#include "../../Model/Axiom.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

class CppAxiom : public Macaroni::Model::Axiom
{
public:
	CppAxiom(const char * msgKey);	

	~CppAxiom();

	virtual const std::string ToString() const;

protected:
	
	virtual bool onDereference();

	virtual void onReference();

private:
	const char * msgKey;
	int refCount;
};

END_NAMESPACE

#endif