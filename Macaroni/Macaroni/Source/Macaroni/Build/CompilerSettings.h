#ifndef MACARONI_BUILD_COMPILERSETTINGS_H
#define MACARONI_BUILD_COMPILERSETTINGS_H

#include "../Environment/Console.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class CompilerSettings
{
public:
	CompilerSettings();	
private:	
	typedef std::vector<const std::string> StrVec;

	std::string additionalCompilerArgs;
	std::string additionalLinkerArgs;
	std::string compilerExe;
	StrVec environmentalVariables;
	std::string firstLinkedObjects;
	StrVec includePaths;
	StrVec linkerPaths;
	std::string oSwitch;
	StrVec paths;
	StrVec preprocessorDirectives; 
};


} }

#endif
