#ifndef MACARONI_BUILD_CPP_CPPFILE_CPP
#define MACARONI_BUILD_CPP_CPPFILE_CPP

#include "CppFile.h"
#include "../../Environment/Process.h"
#include <sstream>

using Macaroni::Environment::Console;
using Macaroni::IO::Path;
using Macaroni::Environment::Process;

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

CppFile::CppFile(boost::filesystem::path srcRoot, boost::filesystem::path srcPath, boost::filesystem::path outputDirectory)
: obj(srcRoot, srcPath), // this is indeed nonsense btw
  src(srcRoot, srcPath)
{	
	std::string::size_type dotIndex = src.ToString().rfind('.');
	std::string objRelativePath = src.ToString().substr(0, dotIndex) + ".obj";
	boost::filesystem::path objFile = outputDirectory / objRelativePath;
	obj = Path(outputDirectory, objFile);
}

void CppFile::Compile(CompilerSettings & settings, const Console & console)
{
	console.Write("Compiling ");
	console.Write(src.GetAbsolutePath());
	console.Write(" ==> ");
	console.WriteLine(obj.GetAbsolutePath());

	std::stringstream args;
	args << "-c \"" << src.GetAbsolutePath() << "\"" 
		 << " -o \"" << obj.GetAbsolutePath() << "\"";
	args << " " << settings.GetAdditionalCompilerArgs();
	for (int i = 0; i < settings.GetIncludePaths().size(); i ++)
	{
		args << " -I\"" << settings.GetIncludePaths()[i] << "\"";
	}

	Process process(settings.GetCompilerExe(), args.str(), settings.GetPaths());
	process.Run(console);	
}

void CppFile::DeleteObjectFile(const Console & console)
{
	console.Write("Deleting ");
	console.WriteLine(obj.GetAbsolutePath());
}

END_NAMESPACE

#endif
