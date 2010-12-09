#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H
#define MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H

// This class was originally defined in Source/Macaroni/Build/CmdLine.mcpp
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
private: 	bool  compile;
private: 	std::string  config;
private: 	Macaroni::Environment::Console &  console;
private: 	bool  debugMode;
private: 	bool  endPrompt;
private: 	bool  generate;
private: 	std::string  inputPath;
private: 	bool  install;
private: 	std::string  luaTestsPath;
private: 	std::string  manifestPath;
private: 	std::string  outputPath;
private: 	bool  saveCppSource;
private: 	bool  startPrompt;
	public: CmdLine(const std::vector<const std::string > &  args, Macaroni::Environment::Console &  console);
	public: ~CmdLine();
private: 	void  directCompile();
public: 	bool  EndPrompt();
public: 	bool  Execute();
private: 	void  parseArg(const std::string &  arg, const std::string &  next);
private: 	void  runLuaTests();
private: 	bool  runManifest();
public: 	inline bool  StartPrompt()	{
		
        return startPrompt;
    
	}
}; // End of class CmdLine

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_H