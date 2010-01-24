#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H

// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { namespace Cpp { 
class CppFile;
} } } // End namespace 

#include <vector>
#include <Macaroni/IO/Path.h>
#include <Macaroni/Environment/Console.h>
#include <boost/filesystem/operations.hpp>
#include <string>
#include <Macaroni/Build/Cpp/CompilerSettings.h>

namespace Macaroni { namespace Build { namespace Cpp { 


/* Public Global Methods */

class CppFile
{
public:
	CppFile(boost::filesystem::path  srcRoot, boost::filesystem::path  srcPath, boost::filesystem::path  outputDirectory);
	bool  Compile(Macaroni::Build::Cpp::CompilerSettings &  settings, const std::vector<const std::string > &  configIncludePaths, const Macaroni::Environment::Console &  console);
	void  CreateObjectFileDirectories();
	void  DeleteObjectFile(const Macaroni::Environment::Console &  console);
	boost::filesystem::path  GetObjectFilePath();
	inline bool  ObjectFileExists() const	{
		
        return obj.Exists();
    
	}
protected:
private:
	Macaroni::IO::Path  obj;
	Macaroni::IO::Path  src;
}; // End of class CppFile

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H