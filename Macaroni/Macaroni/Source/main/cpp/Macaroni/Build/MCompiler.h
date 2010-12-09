#ifndef MACARONI_BUILD_MCOMPILER_H
#define MACARONI_BUILD_MCOMPILER_H

#include "MCompilerOptions.h"
#include "../Model/ContextPtr.h"
#include "../IO/FileSet.h"
#include "../Model/LibraryPtr.h"
#include <vector>

namespace Macaroni { namespace Build {

class MCompiler
{
public:
	bool BuildModel(Macaroni::Model::LibraryPtr library, const std::vector<Macaroni::IO::FileSet> inputFiles);

	void Compile(Macaroni::Model::LibraryPtr library,
		         const MCompilerOptions & options);
};


} }

#endif
