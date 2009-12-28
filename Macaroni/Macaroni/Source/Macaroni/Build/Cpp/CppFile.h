#ifndef MACARONI_BUILD_CPP_CPPFILE_H
#define MACARONI_BUILD_CPP_CPPFILE_H

#include "CompilerSettings.h"
#include "../../Environment/Console.h"
#include <boost/filesystem/operations.hpp>
#include "../../IO/Path.h"
#include <string>
#include <vector>

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

/** A CppFile.  It's root path is either the generated source or normal source
 * directories with its end path the relative path to the file. */
/** TODO: Add stuff such as a timestamp and link to the Macaroni files which
 * created this file, and other stuff like that, in order to do incrementally
 * compilation. */
class CppFile
{
public:
	CppFile(boost::filesystem::path srcRoot, boost::filesystem::path srcPath, boost::filesystem::path outputDirectory);//Path srcPath, Path objPath);
	void Compile(CompilerSettings & settings, const Environment::Console & console);
	void CreateObjectFileDirectories();
	void DeleteObjectFile(const Environment::Console & console);
private:
	IO::Path obj;
	IO::Path src;
};


END_NAMESPACE

#endif
