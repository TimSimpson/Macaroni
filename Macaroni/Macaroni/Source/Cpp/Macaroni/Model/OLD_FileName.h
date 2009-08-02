#ifndef MACARONI_FILENAME_H
#define MACARONI_FILENAME_H

#include "../ME.h"
#include "FileNamePtr.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Represents any file name which could be the Source of Model information. */
class FileName
{
friend void intrusive_ptr_add_ref(FileName * p);
friend void intrusive_ptr_release(FileName * p);

public:	
	static FileNamePtr Create(const std::string & name);
	const std::string & GetName() const;
	int GetReferenceCount() const;
protected:
	FileName(const std::string & name);
	~FileName();
private:	
	std::string name;
	int referenceCount;
};

END_NAMESPACE2

#endif
