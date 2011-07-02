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
#ifndef MACARONI_AXIOM_CPP
#define MACARONI_AXIOM_CPP

#include "Axiom.h"

BEGIN_NAMESPACE2(Macaroni, Model)

bool Axiom::operator== (const Axiom & other) const
{
	return (this->ToString() == other.ToString());
}

void intrusive_ptr_add_ref(Axiom * p)
{
	p->onReference();
}

void intrusive_ptr_release(Axiom * p)
{
	if (!(p->onDereference()))
	{
		delete p;
	}
}

END_NAMESPACE2

#endif


