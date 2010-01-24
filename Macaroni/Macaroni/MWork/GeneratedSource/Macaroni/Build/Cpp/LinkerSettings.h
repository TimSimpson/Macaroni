#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_H

// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { namespace Cpp { 
class LinkerSettings;
} } } // End namespace 

#include <string>
#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <Macaroni/Build/Configuration.h>
#include <vector>

namespace Macaroni { namespace Build { namespace Cpp { 


/* Public Global Methods */

class LinkerSettings
{
public:
	void  AddCompilerLibPaths(const Macaroni::Build::Cpp::CompilerSettings &  compiler);
	void  AddConfigurationLibPaths(const Macaroni::Build::Configuration &  config);
	const std::vector<const std::string > &  GetLibPaths() const;
protected:
private:
	std::vector<const std::string >  libPaths;
}; // End of class LinkerSettings

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_LinkerSettings_H