#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_CPP

#include "CppFile.h"
#include <Macaroni/Build/Cpp/CompilerSettings.h>
#include <Macaroni/Environment/Console.h>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/IO/Path.h>
#include <Macaroni/Environment/Process.h>
#include <string>
#include <Macaroni/Environment/StringPair.h>
#include <sstream>
#include <vector>

using Macaroni::Build::Cpp::CompilerSettings;
using Macaroni::Environment::Console;
using boost::filesystem::create_directories;
using boost::filesystem::path;
using Macaroni::IO::Path;
using Macaroni::Environment::Process;
using std::string;
using Macaroni::Environment::StringPair;
using std::stringstream;
using std::vector;

namespace Macaroni { namespace Build { namespace Cpp { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

CppFile::CppFile(path  srcRoot, path  srcPath, path  outputDirectory)
: obj(srcRoot, srcPath), src(srcRoot, srcPath)
{
	
        std::string::size_type dotIndex = src.ToString().rfind('.');
        std::string objRelativePath = src.ToString().substr(0, dotIndex) + ".obj";
        boost::filesystem::path objFile = outputDirectory / objRelativePath;
        obj = Path(outputDirectory, objFile);
    
}
bool  CppFile::Compile(CompilerSettings &  settings, const std::vector<const std::string > &  configIncludePaths, const Console &  console)
{
	
        CreateObjectFileDirectories();
        console.WriteLine(
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ C++"
        );
        console.Write("Compiling ");
        console.Write(src.GetAbsolutePath());
        console.Write(" ==> ");
        console.WriteLine(obj.GetAbsolutePath());

        boost::filesystem::path objPath(obj.GetAbsolutePath());
        objPath.branch_path();

        std::stringstream args;    
        args << "/Fo" << obj.GetAbsolutePath() << " ";
        args << "/c \"" << src.GetAbsolutePath() << "\" ";
        //args << " \"" << src.GetAbsolutePath() << "\" ";
        args << " " << settings.GetAdditionalCompilerArgs();
        for (unsigned int i = 0; i < settings.GetIncludePaths().size(); i ++)
        {
            args << " -I\"" << settings.GetIncludePaths()[i] << "\"";
        }
        for (unsigned int i = 0; i < configIncludePaths.size(); i ++)
        {
            args << " -I\"" << configIncludePaths[i] << "\"";
        }

        for (unsigned int i = 0; i < settings.GetPreprocessorDirectives().size(); i ++)
        {
            const StringPair & pair = settings.GetPreprocessorDirectives()[i];
            args << " /D \"" << pair.Name << "\"";        
            if (!pair.Value.empty())
            {
                args << "=\"" << pair.Value << "\"";
            }
            args << " ";
        }

        Process process(settings.GetCompilerExe(), args.str(), 
                        objPath.branch_path(), settings.GetPaths(), 
                        settings.GetEnvironmentVariables());
        process.Run(console);    
        
        return ObjectFileExists();
    
}
void  CppFile::CreateObjectFileDirectories()
{
	
        path objPath = path(obj.GetAbsolutePath()).branch_path();
        boost::filesystem::create_directories(objPath);
    
}
void  CppFile::DeleteObjectFile(const Console &  console)
{
	
        console.Write("Deleting ");
        console.WriteLine(obj.GetAbsolutePath());
    
}
path  CppFile::GetObjectFilePath()
{
	
        return boost::filesystem::path(obj.GetAbsolutePath());
    
}
//~<(Skipping inline function "Macaroni::Build::Cpp::CppFile::ObjectFileExists")

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Cpp_CppFile_CPP