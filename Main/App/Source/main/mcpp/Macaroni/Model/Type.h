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
#ifndef MACARONI_MODEL_TYPE_H
#define MACARONI_MODEL_TYPE_H

#include "../ME.h"
#include "Node.h"
#include "TypePtr.h"
#include <Macaroni/Model/TypeModifiers.h>
#include "TypeArgumentPtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)


//TODO: This HAS to start living in the Context.  It is a memory leak... can't
// believe I didn't notice it at the time.
/** Represents a Type.  A type does not exist in the Node system, but does
 * point to a Node.
 * This may also store a list of TypeArguments, where each element points to
 * a Node that relates to the type node, with additional nodes offering
 * argument data. */
class Type
{
public:
	Type(NodePtr type, TypeModifiers modifiers);
	Type(NodePtr type, TypeModifiers modifiers, TypeArgumentListPtr typeArguments);

	~Type();

	bool operator== (const Type & other) const;

	void DescribeDifferences(const TypePtr info, std::stringstream & stream) const;

	NodePtr GetNode() const;

	TypeArgumentListPtr GetTypeArguments() const;

	inline bool IsConst() const
	{
		return modifiers.Const();
	}

	inline bool IsConstPointer() const
	{
		return modifiers.ConstPointer();
	}

	/** Light means that the type is a "light dependency", which means it can
	 * be satisfied with a forward reference. The '~light' keyword is used
	 * for this. It useful with complex template types Macaroni isn't smart
	 * enough to understand which end up just needing forward references. */
	inline bool IsLight() const
	{
		return modifiers.Light();
	}

	inline bool IsPointer() const
	{
		return modifiers.Pointer();
	}

	inline bool IsReference() const
	{
		return modifiers.Reference();
	}

	inline bool IsRvalueReference() const
	{
		return modifiers.RvalueReference();
	}

	const TypeModifiers & GetModifiers() const
	{
		return modifiers;
	}



	static bool ListContains(TypeListPtr list, TypePtr target);

	static bool ListIsEqual(TypeListPtr list1, TypeListPtr list2);

private:
	NodePtr type;
	TypeArgumentListPtr typeArguments;
	TypeModifiers modifiers;

};

END_NAMESPACE2

#endif
