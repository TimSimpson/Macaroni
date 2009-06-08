#ifndef MACARONI_SOURCE_H
#define MACARONI_SOURCE_H

#include "../ME.h"
#include "FileName.lh"
#include "Source.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

/** A location in a file which relates to the Model in some way. */
class Source
{
friend void intrusive_ptr_add_ref(Source * p);
friend void intrusive_ptr_release(Source * p);
public:
	static SourcePtr Create(const FileNamePtr fileName, int line);
	int GetLineNumber() const;
	FileNamePtr GetFileName() const;
	int GetReferenceCount() const;
	std::string ToString() const;
private:	
	Source(const FileNamePtr fileName, int line);
	~Source();
	FileNamePtr fileName;
	int lineNumber;
	int referenceCount;
};

END_NAMESPACE2

#endif
