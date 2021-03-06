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
#ifndef MACARONI_MODEL_ATTRIBUTETABLE_CPP
#define MACARONI_MODEL_ATTRIBUTETABLE_CPP

#include "AnnotationTable.h"
#include <boost/foreach.hpp>

#include <Macaroni/Model/Context.h>
#include <Macaroni/Exception.h>
#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Model)

AnnotationTable::AnnotationTable(Context & context)
:	context(context), home(nullptr)
{
}

AnnotationTable::AnnotationTable(Node & home)
:	context(*(home.GetContext().get())), home(&home)
{
} 

AnnotationTable::~AnnotationTable()
{
}

AnnotationValuePtr AnnotationTable::Add(AnnotationValueInternalPtr value) 
{
	values.push_back(value);
	return AnnotationValuePtr(value.get());
}

//TODO: Do not allow :: in these names!!!

AnnotationValuePtr AnnotationTable::Add(const std::string & name, const bool value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AnnotationValueInternalPtr newValue(new AnnotationValue(newName, value, reason));
	return Add(newValue);	
}
	
AnnotationValuePtr AnnotationTable::Add(const std::string & name, const double value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AnnotationValueInternalPtr newValue(new AnnotationValue(newName, value, reason));
	return Add(newValue);	
}

AnnotationValuePtr AnnotationTable::Add(const std::string & name, NodePtr value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AnnotationValueInternalPtr newValue(new AnnotationValue(newName, value, reason));
	return Add(newValue);	
}

AnnotationValuePtr AnnotationTable::Add(const std::string & name, const std::string & value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AnnotationValueInternalPtr newValue(new AnnotationValue(newName, value, reason));
	return Add(newValue);	
}

AnnotationValuePtr AnnotationTable::Add(const std::string & name, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AnnotationValueInternalPtr newValue(new AnnotationValue(newName, reason));
	return Add(newValue);	
}

bool AnnotationTable::allowNestedNames() const
{
	return home == nullptr;
}

AnnotationValuePtr AnnotationTable::Get(int index) const
{
	return AnnotationValuePtr(values[index].get());
}

AnnotationValuePtr AnnotationTable::GetByName(const NodePtr & name) const
{
	BOOST_FOREACH(AnnotationValueInternalPtr ptr, values)
	{
		if (ptr->GetName()->GetFullName() == name->GetFullName())
		{
			return AnnotationValuePtr(ptr.get());
		}
	}
	return AnnotationValuePtr();
}

AnnotationValuePtr AnnotationTable::GetByName(const std::string & name) const
{
	NodePtr node = this->getHome().Find(name);
	if (!node) 
	{
		return AnnotationValuePtr();
	}
	else
	{
		return GetByName(node);
	}	
}

Node & AnnotationTable::getHome() const
{
	if (home == nullptr)
	{
		Node & h = *(context.GetRoot().get());
		return h;
	}
	else
	{
		Node & h = *home;
		return h;
	}
}

NodePtr AnnotationTable::GetHomeNode()
{
	return NodePtr(&(getHome()));
}

void intrusive_ptr_add_ref(AnnotationTable * p)
{
	intrusive_ptr_add_ref(&p->context);
}

void intrusive_ptr_release(AnnotationTable * p)
{
	intrusive_ptr_release(&p->context);
}

END_NAMESPACE2

#endif

