#ifndef MACARONI_SOURCE_H
#define MACARONI_SOURCE_H

#include "../ME.h"
#include <Macaroni/Model/FileNamePtr.h>
#include "SourcePtr.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** A location in a file which relates to the Model in some way. */
class Source
{
friend void intrusive_ptr_add_ref(Source * p);
friend void intrusive_ptr_release(Source * p);
public:
	bool operator== (const Source & other) const;
	static SourcePtr Create(const FileNamePtr fileName, int line, int column);
	int GetColumn() const;
	int GetLine() const;
	FileNamePtr GetFileName() const;
	int GetReferenceCount() const;
	/** Creates a new Source object at the given line number. */
	SourcePtr JumpToLine(int lineNumber) const;
	std::string ToString() const;
private:	
	Source(const FileNamePtr fileName, int line, int column);
	~Source();
	int column;
	FileNamePtr fileName;
	int lineNumber;
	int referenceCount;
};

END_NAMESPACE2

#endif
