#ifndef MACARONI_SOURCE_CPP
#define MACARONI_SOURCE_CPP

#include "../ME.h"
#include "FileName.h"
#include "Source.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Source::Source(const FileNamePtr fileName, int line)
:fileName(fileName), lineNumber(line), referenceCount(0)
{
}

Source::~Source()
{
}

SourcePtr Source::Create(const FileNamePtr fileName, int line)
{
	return SourcePtr(new Source(fileName, line));
}

int Source::GetLineNumber() const
{
	return lineNumber;
}

FileNamePtr Source::GetFileName() const
{
	return fileName;
}

int Source::GetReferenceCount() const
{
	return referenceCount;
}

SourcePtr Source::JumpToLine(int newLineNumber) const
{
	return Create(fileName, newLineNumber);
}

void intrusive_ptr_add_ref(Source * p)
{
	p->referenceCount ++;
}

void intrusive_ptr_release(Source * p)
{
	p->referenceCount --;
}

std::string Source::ToString() const
{
	std::stringstream ss;
	ss << this->GetFileName()->GetName() << ", line " << this->GetLineNumber();
	return ss.str();	
}

END_NAMESPACE2

#endif
