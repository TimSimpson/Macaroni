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
#ifndef MACARONI_AXIOM_H
#define MACARONI_AXIOM_H

#include "../ME.h"
#include "AxiomPtr.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents some rule or principal which client code uses to create or alter
 * the Model. */
class Axiom
{
friend void intrusive_ptr_add_ref(Axiom * p);
friend void intrusive_ptr_release(Axiom * p);

public:

	virtual const std::string ToString() const = 0;
	
	virtual bool operator== (const Axiom & other) const;
	
protected:

	Axiom(){}

	virtual ~Axiom(){}
		
	/** Called when dereferenced by a smart pointer.  Smart pointer will delete
	 * the object if false is returned. */
	virtual bool onDereference() = 0;

	/** Called when referenced by a smart pointer. */
	virtual void onReference() = 0;

};

END_NAMESPACE2

#endif
