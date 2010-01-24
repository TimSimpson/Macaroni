#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H

// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Build { 
class CmdLine;
} } // End namespace 

#include <Macaroni/Environment/Console.h>
#include <string>
#include <vector>

namespace Macaroni { namespace Build { 


/* Public Global Methods */

class CmdLine
{
public:
	CmdLine(const std::vector<const std::string > &  args, Macaroni::Environment::Console &  console);
	bool  EndPrompt();
	void  Execute();
protected:
private:
	bool  compile;
	std::string  config;
	Macaroni::Environment::Console &  console;
	bool  debugMode;
	bool  endPrompt;
	bool  generate;
	std::string  inputPath;
	bool  install;
	std::string  luaTestsPath;
	std::string  manifestPath;
	std::string  outputPath;
	void  directCompile();
	void  parseArg(const std::string &  arg, const std::string &  next);
	void  runLuaTests();
	void  runManifest();
}; // End of class CmdLine

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H