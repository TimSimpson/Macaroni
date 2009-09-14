#ifndef MACARONI_GENERATOR_DYNAMICGENERATORS_H
#define MACARONI_GENERATOR_DYNAMICGENERATORS_H

#include "../ME.h"
#include "../Model/Context.h"
#include "../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE2(Macaroni, Generator)

void RunDynamicGenerators(Model::ContextPtr context, const boost::filesystem::path & rootPath);

END_NAMESPACE2

#endif

