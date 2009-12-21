#ifndef MACARONI_BUILD_CMDLINE_H
#define MACARONI_BUILD_CMDLINE_H

#include "../Environment/Console.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

/** 
 * Handles a typical command line invokation. 
 * Supports running arbitrary Lua (the unit tests),
 * direct calling of the Macaroni compiler on specific targets,
 * or building of a Module in the target directory.
 */
class CmdLine
{
public:
	CmdLine(const std::vector<const std::string> & args,
			Macaroni::Environment::Console & console);
	void Execute();
private:	
	Macaroni::Environment::Console & console;
	bool debugMode;
	void directCompile();
	bool endPrompt;
	std::string inputPath;	
	std::string luaTestsPath;
	std::string outputPath;
	void parseArg(const std::string & arg, const std::string & next);
	void runLuaTests();
	void runManifest();
};


} }

#endif
