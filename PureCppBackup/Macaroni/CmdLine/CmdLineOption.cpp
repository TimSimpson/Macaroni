#ifndef MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_CPP

#include "CmdLineOption.h"
#include <Macaroni/Build/Builder.h>
#include <Macaroni/Environment/Console.h>
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/ContextPtr.h>
#include <Macaroni/IO/FileSet.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/Library.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/Environment/LuaEnvironment.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/MCompiler.h>
#include <Macaroni/Build/MCompilerOptions.h>
#include <Macaroni/Environment/Messages.h>
#include <string>
#include <sstream>
#include <vector>

using Macaroni::Build::Builder;
using Macaroni::Environment::Console;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::IO::FileSet;
using Macaroni::Exception;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Environment::LuaEnvironment;
using Macaroni::Build::Manifest;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Macaroni::Environment::Messages;
using std::string;
using std::stringstream;
using std::vector;

namespace Macaroni { namespace CmdLine { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

CmdLineOption::CmdLineOption(const string  name, const string  description)
: name(name), description(description)
{
	
    
}
//~<(Skipping inline function "Macaroni::CmdLine::CmdLineOption::GetDescription")
//~<(Skipping inline function "Macaroni::CmdLine::CmdLineOption::GetName")
void  CmdLineOption::SetDescription(const string &  value)
{
	
        description = value;
    
}
void  CmdLineOption::SetName(const string &  value)
{
	
        name = value;
    
}

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_CmdLine_CmdLineOption_CPP