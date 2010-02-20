#ifndef MACARONI_GENERATOR_DEBUGENUMERATOR_H
#define MACARONI_GENERATOR_DEBUGENUMERATOR_H

#include "../ME.h"
#include "../Model/Context.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE2(Macaroni, Generator)

class DebugEnumerator
{
public:
	DebugEnumerator();
	
	~DebugEnumerator();
	
	void Iterate(Model::ContextPtr context, std::ostream & out);

private:

};

END_NAMESPACE2

#endif

