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
#ifndef MACARONI_MODEL_CPP_TYPEINFO_CPP
#define MACARONI_MODEL_CPP_TYPEINFO_CPP

#include "TypeInfo.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

TypeInfo::TypeInfo()
	:IsConst(false),
	 IsConstPointer(false),
	 IsPointer(false), 
	 IsReference(false),
	 Node()
{
}

bool TypeInfo::operator==(const TypeInfo & other) const 
{
	return IsConst == other.IsConst && IsConstPointer == other.IsConstPointer
		&& IsPointer == other.IsPointer && IsReference == other.IsReference
		&& Node == other.Node;
}

void TypeInfo::DescribeDifferences(const TypeInfo & info, std::stringstream & stream) const
{
	if (this->IsConst && !info.IsConst)
	{
		stream << "Previous definition was const.";
	}
	if (this->IsConstPointer && !info.IsConstPointer)
	{
		stream << "Previous definition was const pointer.";
	}
	if (this->IsPointer && !info.IsPointer)
	{
		stream << "Previous definition was pointer.";
	}
	if (this->IsReference && !info.IsReference)
	{
		stream << "Previous definition was reference.";
	}
}

END_NAMESPACE

#endif
