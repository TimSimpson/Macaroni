#ifndef MACARONI_BUILD_CMDLINE_CPP
#define MACARONI_BUILD_CMDLINE_CPP

#include "../ME.h"
#include "Builder.h"
#include "CmdLine.h"
#include "MCompiler.h"
#include "MCompilerOptions.h"
#include "Manifest.h"
#include "../Environment/Console.h"
#include "../Exception.h"
#include "../Environment/LuaEnvironment.h"
#include "../Environment/Messages.h"
#include "../Gestalt/FileSystem/FileSet.h"
#include <vector>
#include <string>
#include <sstream>

using Macaroni::Environment::Console;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Gestalt::FileSystem::FileSet;
using Macaroni::Environment::Messages;

namespace Macaroni { namespace Build {


CmdLine::CmdLine(const std::vector<const std::string> & args, Console & console)
: config(""),
  console(console),
  debugMode(false),
  endPrompt(false),
  inputPath(""),
  luaTestsPath(""),
  manifestPath(""),
  outputPath("")
{
	for(unsigned int i = 0; i < args.size(); i ++)
	{
		const std::string arg = args[i];
		const std::string nextArg;
		if (i < args.size() - 1) {
			const std::string nextArg = args[i + 1];
			parseArg(arg, nextArg);	
		} else {
			parseArg(arg, "");
		}				
	}	
}

void CmdLine::directCompile()
{
	if (!inputPath.empty() && outputPath.empty())
	{
		console.Write("Missing option outputDirectory.");
	}
	else if (inputPath.empty() && !outputPath.empty())
	{
		console.Write("Missing option inputDirectory.");
	}
	else
	{
		console.Write("Executing compiler directly (no manifest).");
		try
		{
			const std::vector<const std::string> generators;
			FileSet input(boost::filesystem::path(inputPath), "\\.mcpp$");
			MCompilerOptions options(input, 
									 boost::filesystem::path(outputPath),
									 generators);
			MCompiler compiler;
			compiler.Compile(options);
		}
		catch(Macaroni::Exception ex)
		{
			console.Write("UNHANDLED EXCEPTION:\n");
			console.WriteLine(ex.GetMessage());
			console.WriteLine(ex.GetSource());
		}
		catch(Macaroni::Exception * ex)
		{
			console.Write("UNHANDLED EXCEPTION:\n");
			console.WriteLine(ex->GetMessage());
			console.WriteLine(ex->GetSource());
		}
	}
}

bool CmdLine::EndPrompt()
{
	return endPrompt;
}

void CmdLine::Execute()
{
	if (!luaTestsPath.empty())
	{
		//runLuaTests();
	}

	if (!(inputPath.empty() && outputPath.empty()))
	{
		directCompile();
	}		
	else 
	{
		runManifest();// TODO: Read manifest from local directory and attempt build.
	}
}

void CmdLine::parseArg(const std::string & arg, const std::string & next)
{
	if (arg == "-configuration")
	{
		if (next.empty())
		{
			config = "";
		}
		else
		{
			config = next;
		}
	}
	else if (arg == "-debug")
	{
		debugMode = true;
	}
	else if (arg == "-luaTests")
	{
		if (next.empty())
		{
			console.Write("No input specified for luaTests command.");
		}
		else
		{
			luaTestsPath = next;			
		}
	}
	else if (arg == "-inputDirectory")
	{
		if (next.empty())
		{
			console.Write("No input directory given following \"-inputDirectory\" arg.");
		}
		else
		{
			inputPath = next;
		}
	}
	else if (arg == "-manifest")
	{
		if (next.empty())
		{
			console.WriteLine("No file given following \"manifest\" arg.");
		}
		else
		{
			manifestPath = next;
		}
	}
	else if (arg == "-outputDirectory")
	{
		if (next.empty())
		{
			console.Write("No output directory given following \"-outputDirectory\" arg.");
		}
		else
		{
			outputPath = next;
		}
	}
	else if (arg == "-endprompt")
	{
		endPrompt = true;
	}
}

void CmdLine::runLuaTests()
{
	console.WriteLine("Running arbitrary LUA files.");
	Macaroni::Environment::LuaEnvironment lua;
	lua.ParseFile("Main.lua"); 
	lua.Run();
}

void CmdLine::runManifest()
{
	if (manifestPath.empty())
	{
		manifestPath = "manifest.lua";
	}

	if (config.empty())
	{
		config = "all";
	}

	boost::filesystem::path manifestFilePath(manifestPath);	
	Manifest manifest(manifestFilePath);

	const Configuration * mConfig = manifest.GetConfiguration(config);
	if (mConfig == nullptr)
	{
		console.Write("Could not find config \"");
		console.Write(config);
		console.Write("\" in manifest.");
		return;
	}
	const Configuration & mRefConfig = *mConfig;

	Builder builder(manifest, mRefConfig, console);
	builder.Execute();
}

} } // end namespace

#endif
