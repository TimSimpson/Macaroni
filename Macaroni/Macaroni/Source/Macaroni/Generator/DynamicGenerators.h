#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_H
#define MACARONI_GENERATOR_DYNAMICGENERATORS_H

#include "../ME.h"
#include "../Model/Context.h"
#include "../../Gestalt/FileSystem/FileSet.h"
#include "../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE2(Macaroni, Generator)

boost::filesystem::path GetGeneratorsPath();

/** Looks for the Generator file first in the given FileSet of local directories
 * (it appends the file name to each one) and then in the Generator path. 
 */
boost::filesystem::path ResolveGeneratorPath(const Gestalt::FileSystem::FileSet & localDirs,
											 const std::string & guess);

void RunDynamicGenerator(Model::ContextPtr context, 
						  const boost::filesystem::path & rootPath,
						  const boost::filesystem::path & generatorFilePath);

void RunDynamicGenerators(Model::ContextPtr context, const boost::filesystem::path & rootPath);

END_NAMESPACE2

#endif

