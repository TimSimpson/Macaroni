//~ MACARONI_FILE_OVERWRITE_ALLOWED

#ifndef MACARONI_FILENAME_CPP
#define MACARONI_FILENAME_CPP

#include "FileName.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

FileName::FileName(const std::string & name)
:name(name), referenceCount(0)
{
}

FileName::~FileName()
{
}

FileNamePtr FileName::Create(const std::string & name)
{
	return FileNamePtr(new FileName(name));
}

const std::string & FileName::GetName() const
{
	return name;
}

int FileName::GetReferenceCount() const
{
	return referenceCount;
}

void intrusive_ptr_add_ref(FileName * p)
{
	p->referenceCount ++;
}

void intrusive_ptr_release(FileName * p)
{
	p->referenceCount --;
	if (p->referenceCount < 1)
	{
		delete p;
	}
}	

END_NAMESPACE2

#endif
