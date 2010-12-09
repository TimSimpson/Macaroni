#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H

// This class was originally defined in Source/Macaroni/Build/Cpp/CppFile.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { namespace Cpp { 
class CppFile;
} } } // End namespace 

#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <vector>
#include <string>
#include <Macaroni/Environment/Console.h>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/IO/Path.h>

namespace Macaroni { namespace Build { namespace Cpp { 


/* Public Global Methods */

class CppFile
{
private: 	Macaroni::IO::Path  obj;
private: 	Macaroni::IO::Path  src;
	public: CppFile(boost::filesystem::path  srcRoot, boost::filesystem::path  srcPath, boost::filesystem::path  outputDirectory);
public: 	bool  Compile(Macaroni::Build::Cpp::CompilerSettings &  settings, const std::vector<const std::string > &  configIncludePaths, const Macaroni::Environment::Console &  console);
public: 	void  CreateObjectFileDirectories();
public: 	void  DeleteObjectFile(const Macaroni::Environment::Console &  console);
public: 	boost::filesystem::path  GetObjectFilePath();
public: 	bool  IsObjectFileOutOfDate() const;
public: 	inline bool  ObjectFileExists() const	{
		
        return obj.Exists();
    
	}
}; // End of class CppFile

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_H