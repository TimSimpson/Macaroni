#ifndef MACARONI_BUILD_MCOMPILER_H
#define MACARONI_BUILD_MCOMPILER_H

#include "MCompilerOptions.h"
#include "../Model/Context.lh"
#include "../Model/Library.lh"

namespace Macaroni { namespace Build {

class MCompiler
{
public:
	void Compile(Macaroni::Model::LibraryPtr library,
		         const MCompilerOptions & options);
};


} }

#endif
