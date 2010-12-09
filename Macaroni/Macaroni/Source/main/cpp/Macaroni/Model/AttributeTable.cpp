#ifndef MACARONI_MODEL_ATTRIBUTETABLE_CPP
#define MACARONI_MODEL_ATTRIBUTETABLE_CPP

#include "AttributeTable.h"
#include <boost/foreach.hpp>

#include "Context.h"
#include <Macaroni/Exception.h>
#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Model)

AttributeTable::AttributeTable(Context & context)
:	context(context), home(nullptr)
{
}

AttributeTable::AttributeTable(Node & home)
:	context(*(home.GetContext().get())), home(&home)
{
} 

AttributeTable::~AttributeTable()
{
}

AttributeValuePtr AttributeTable::Add(AttributeValueInternalPtr value) 
{
	values.push_back(value);
	return AttributeValuePtr(value.get());
}

//TODO: Do not allow :: in these names!!!

AttributeValuePtr AttributeTable::Add(const std::string & name, const bool value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AttributeValueInternalPtr newValue(new AttributeValue(newName, value, reason));
	return Add(newValue);	
}
	
AttributeValuePtr AttributeTable::Add(const std::string & name, const double value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AttributeValueInternalPtr newValue(new AttributeValue(newName, value, reason));
	return Add(newValue);	
}

AttributeValuePtr AttributeTable::Add(const std::string & name, NodePtr value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AttributeValueInternalPtr newValue(new AttributeValue(newName, value, reason));
	return Add(newValue);	
}

AttributeValuePtr AttributeTable::Add(const std::string & name, const std::string & value, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AttributeValueInternalPtr newValue(new AttributeValue(newName, value, reason));
	return Add(newValue);	
}

AttributeValuePtr AttributeTable::Add(const std::string & name, const ReasonPtr & reason)
{
	NodePtr newName = getHome().FindOrCreate(name);
	AttributeValueInternalPtr newValue(new AttributeValue(newName, reason));
	return Add(newValue);	
}

bool AttributeTable::allowNestedNames() const
{
	return home == nullptr;
}

AttributeValuePtr AttributeTable::Get(int index) const
{
	return AttributeValuePtr(values[index].get());
}

AttributeValuePtr AttributeTable::GetByName(const NodePtr & name) const
{
	BOOST_FOREACH(AttributeValueInternalPtr ptr, values)
	{
		if (ptr->GetName()->GetFullName() == name->GetFullName())
		{
			return AttributeValuePtr(ptr.get());
		}
	}
	return AttributeValuePtr();
}

AttributeValuePtr AttributeTable::GetByName(const std::string & name) const
{
	NodePtr node = this->getHome().Find(name);
	if (!node) 
	{
		return AttributeValuePtr();
	}
	else
	{
		return GetByName(node);
	}	
}

Node & AttributeTable::getHome() const
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

NodePtr AttributeTable::GetHomeNode()
{
	return NodePtr(&(getHome()));
}

void intrusive_ptr_add_ref(AttributeTable * p)
{
	intrusive_ptr_add_ref(&p->context);
}

void intrusive_ptr_release(AttributeTable * p)
{
	intrusive_ptr_release(&p->context);
}

END_NAMESPACE2

#endif

