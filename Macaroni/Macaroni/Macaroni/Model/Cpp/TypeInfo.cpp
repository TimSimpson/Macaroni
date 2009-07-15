#ifndef MACARONI_MODEL_CPP_TYPEINFO_CPP
#define MACARONI_MODEL_CPP_TYPEINFO_CPP

#include "TypeInfo.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

TypeInfo::TypeInfo()
	:IsConst(false),
	 IsConstPointer(false),
	 IsPointer(false), 
	 IsReference(false),
	 Type()
{
}

bool TypeInfo::operator==(const TypeInfo & other) const 
{
	return IsConst == other.IsConst && IsConstPointer == other.IsConstPointer
		&& IsPointer == other.IsPointer && IsReference == other.IsReference
		&& Type == other.Type;
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
