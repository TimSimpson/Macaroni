#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_CPP

#include "LinkerSettings.h"
#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <Macaroni/Build/Configuration.h>
#include <Macaroni/Build/ConfigurationId.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/ME.h>
#include <string> 
#include <sstream>
#include <vector>

using Macaroni::Build::Cpp::CompilerSettings; 
using Macaroni::Build::Configuration;
using Macaroni::Build::ConfigurationId;  
using Macaroni::Exception;
using Macaroni::Build::Manifest;
using std::string;
using std::stringstream; 
using std::vector;

namespace Macaroni { namespace Build { namespace Cpp { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

void  LinkerSettings::AddCompilerLibPaths(const CompilerSettings &  compiler)
{
	
        libPaths.clear();
        for (unsigned int i = 0; i < compiler.GetLinkerLibraryPaths().size(); i ++)
        {
            libPaths.push_back(compiler.GetLinkerLibraryPaths()[i]);
        }         
    
}
void  LinkerSettings::AddConfigurationLibPaths(const Configuration &  config)
{
	
        const vector<const string> configLibs = config.GetLinkerLibraries();
        for (unsigned int j = 0; j < configLibs.size(); j ++)
        {
            libPaths.push_back(configLibs[j]);
        }

        const vector<const ConfigurationId> & dependencies = config.GetDependencies();
        for (unsigned int i = 0; i < dependencies.size(); i ++)
        {
            const ConfigurationId & cId = dependencies[i];
            Manifest dm(cId.GetManifestId().FindFinalManifestFile());    
            const Configuration * dConfig = dm.GetConfiguration(cId.GetName());
            if (dConfig == nullptr)
            {
                stringstream ss;
                ss << "Could not find configuration \"" 
                    << cId.GetName() << " for dependency (" 
                    << cId.GetManifestId().GetGroup() << ", "
                    << cId.GetManifestId().GetName() << ", "
                    << cId.GetManifestId().GetVersion() 
                    << ").";
                throw Macaroni::Exception(ss.str().c_str());
            }
            AddConfigurationLibPaths(*dConfig);
        }        
    
}
const std::vector<const std::string > &  LinkerSettings::GetLibPaths() const
{
	
        return libPaths;
    
}

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_CPP