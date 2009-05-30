#ifndef MACARONI_MEMORYBLOB_H
#define MACARONI_MEMORYBLOB_H

#include "ME.h"

class MemoryBlob;

class MemoryBlob
{
public:
	MemoryBlob();

	void AddReference();

	/** refCount <= 0 */
	bool RemoveReference();

private:
	MemoryBlob(const MemoryBlob & other);
	void operator=(const MemoryBlob & other);
	~MemoryBlob();
	
	int referenceCount;
};

#endif

