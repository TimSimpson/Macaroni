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
#ifndef MACARONI_MODEL_MEMBER_H
#define MACARONI_MODEL_MEMBER_H

#include "../ME.h"
#include "Cpp/NamespacePtr.h"
#include <Macaroni/Model/Element.h>

BEGIN_NAMESPACE2(Macaroni, Model)

//TODO: Possibly rename this to "Element"?
class Member : public Element
{
friend class Node;
friend void intrusive_ptr_add_ref(Member * p);
friend void intrusive_ptr_release(Member * p);
public:

	/** Returns true if a Member of this type can be legally nested under the
	 * type of the given member. */
	bool CanBeChildOf(const Member * other) const;

	/** Returns true if the given node is used to define type information for
	 * this node. */
	//TODO: 2009-10-03- I just saw this and suddenly I HATE it.  The meaning
	//would seem to be too variant between types.  Ugh!
	virtual bool DoesDefinitionReference(NodePtr node) const;

	//virtual const char * GetTypeName() const = 0;

	/** Finds a parent namespace by iterating backwards, skipping unknown nodes. */
	Cpp::NamespacePtr FindClosestParentNamespace() const;

protected:
	Member(Node * node, const char * typeName, const ReasonPtr reasonCreated);

	//// Check all children to see if they can legally be children of this object.
	//void assertChildrenAreLegal();

	//// Subclasses should call this to ensure everything happened as intended.
	//void assertValidConstruction();

	/** This is called from the public method. */
	virtual bool canBeChildOf(const Member * other) const = 0;

	//Node * getNode() const;

	//virtual construct() = 0;

	//void setNode(Node * node);

	//void setReasonCreation(const ReasonPtr reason);

private:
	//Node * node;

	//ReasonPtr reasonCreated;
};

END_NAMESPACE2

#endif

