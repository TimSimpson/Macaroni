#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_H
#define MACARONI_GENERATOR_DYNAMICGENERATORS_H

#include "../ME.h"
#include "../Model/Context.h"
#include "../IO/FileSet.h"
#include <Macaroni/Build/_.h>
#include "../Model/Library.h"
#include "../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/Environment/StringPair.h>

BEGIN_NAMESPACE2(Macaroni, Generator)

boost::filesystem::path GetGeneratorsPath();

/** Looks for the Generator file first in the given FileSet of local directories
 * (it appends the file name to each one) and then in the Generator path. 
 */
boost::filesystem::path ResolveGeneratorPath(const std::vector<const std::string> & srcDirs, 
						  const std::string & name);

boost::filesystem::path ResolveGeneratorPath(const std::vector<Macaroni::IO::FileSet> & srcDirs,
											 const std::string & guess);

void RunDynamicGenerator(Model::LibraryPtr library, 
						  const boost::filesystem::path & rootPath,
						  const boost::filesystem::path & generatorFilePath);

std::string RunDynamicGenerator(const boost::filesystem::path & generatorFilePath,
						 Macaroni::Build::BuildContextPtr buildContext,
						 const std::string & methodName,
						  const std::vector<Macaroni::Environment::StringPair> & arguments);

void RunDynamicGenerators(Model::LibraryPtr library, const boost::filesystem::path & rootPath);

END_NAMESPACE2

#endif

