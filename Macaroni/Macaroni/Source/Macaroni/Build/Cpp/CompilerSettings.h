#ifndef MACARONI_BUILD_CPP_COMPILERSETTINGS_H
#define MACARONI_BUILD_CPP_COMPILERSETTINGS_H

#include "../../Environment/Console.h"
#include <boost/filesystem/operations.hpp>
#include <string>
#include <vector>

BEGIN_NAMESPACE(Macaroni, Build, Cpp)

class CompilerSettings
{
public:
	CompilerSettings();	

	CompilerSettings(const boost::filesystem::path & filePath);	

	inline const std::string & GetAdditionalCompilerArgs() const
	{
		return additionalCompilerArgs;
	}

	inline const std::string & GetCompilerExe() const
	{
		return compilerExe;
	}

	inline const std::vector<const std::string> & GetIncludePaths() const 
	{
		return includePaths;
	}

	inline const std::string & GetLinkerExe() const
	{
		return linkerExe;
	}

	inline const std::vector<const std::string> & GetPaths() const 
	{
		return paths;
	}

private:	
	typedef std::vector<const std::string> StrVec;

	std::string additionalCompilerArgs;
	std::string additionalLinkerArgs;
	std::string compilerExe;
	StrVec environmentVariables;
	std::string firstLinkedObjects;
	StrVec includePaths;
	std::string linkerExe;
	StrVec linkerPaths;
	std::string oSwitch;
	StrVec paths;
	StrVec preprocessorDirectives; 
};


END_NAMESPACE

#endif
