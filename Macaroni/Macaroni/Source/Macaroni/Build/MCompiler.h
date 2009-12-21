#ifndef MACARONI_BUILD_MCOMPILER_H
#define MACARONI_BUILD_MCOMPILER_H

#include "MCompilerOptions.h"

namespace Macaroni { namespace Build {

class MCompiler
{
public:
	void Compile(const MCompilerOptions & options);
};


} }

#endif
