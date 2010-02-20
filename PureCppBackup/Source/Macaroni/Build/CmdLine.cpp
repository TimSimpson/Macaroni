#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_CPP

#include "CmdLine.h"
#include <Macaroni/Build/Builder.h>
#include <Macaroni/Environment/Console.h>
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/ContextPtr.h>
#include <Macaroni/IO/FileSet.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/Library.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/Environment/LuaEnvironment.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/MCompiler.h>
#include <Macaroni/Build/MCompilerOptions.h>
#include <Macaroni/Environment/Messages.h>
#include <string>
#include <sstream>
#include <vector>

using Macaroni::Build::Builder;
using Macaroni::Environment::Console;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::IO::FileSet;
using Macaroni::Exception;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Environment::LuaEnvironment;
using Macaroni::Build::Manifest;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Macaroni::Environment::Messages;
using std::string;
using std::stringstream;
using std::vector;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

CmdLine::CmdLine(const std::vector<const std::string > &  args, Console &  console)
: compile(false), config(""), console(console), debugMode(false), endPrompt(false), generate(false), inputPath(""), install(false), luaTestsPath(""), manifestPath(""), outputPath(""), saveCppSource(false)
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
CmdLine::~CmdLine()
{
	
        outputPath = "";
        config = "";
        luaTestsPath = "";
    
}
void  CmdLine::directCompile()
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
                std::vector<FileSet> input;
                input.push_back(FileSet(boost::filesystem::path(inputPath), "\\.mcpp$"));
                MCompilerOptions options(input, 
                                         boost::filesystem::path(outputPath),
                                         generators);
                MCompiler compiler;
                ContextPtr context(new Context(std::string("%ROOT%")));
                LibraryPtr library = context->CreateLibrary("Command Line", "???");
                compiler.Compile(library, options);
            }
            catch(Macaroni::Exception & ex)
            {
                console.Write("UNHANDLED EXCEPTION:\n");
                console.WriteLine(ex.GetMessage());
                console.WriteLine(ex.GetSource());
            } 
        }
    
}
bool  CmdLine::EndPrompt()
{
	
        return endPrompt;
    
}
void  CmdLine::Execute()
{
	
        if (!luaTestsPath.empty())
        {
            runLuaTests();
        }
        else if (!(inputPath.empty() && outputPath.empty()))
        {
            directCompile();
        }        
        else 
        {
            runManifest();// TODO: Read manifest from local directory and attempt build.
        }
    
}
void  CmdLine::parseArg(const string &  arg, const string &  next)
{
	
        if (arg == "configuration")
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
        else if (arg == "compile")
        {
            compile = true;
        }
        else if (arg == "debug")
        {
            debugMode = true;
        }
        else if (arg == "generate")
        {
            generate = true;
        }
        else if (arg == "install")
        {
            install = true;
        }
        else if (arg == "luaTests")
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
        else if (arg == "inputDirectory")
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
        else if (arg == "manifest")
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
        else if (arg == "outputDirectory")
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
        else if (arg == "endprompt")
        {
            endPrompt = true;
        }
        else if (arg == "saveCppSource")
        {
            saveCppSource = true;
        }
    
}
void  CmdLine::runLuaTests()
{
	
        console.WriteLine("Running arbitrary LUA files.");
        Macaroni::Environment::LuaEnvironment lua;
        lua.SetPackageDirectory(luaTestsPath);

        std::stringstream ss;
        ss << luaTestsPath << "/Tests.lua";
        lua.ParseFile(ss.str()); 
        lua.Run();
    
}
void  CmdLine::runManifest()
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

        ContextPtr context(new Context("%ROOT%"));
        Builder builder(context, manifest, mRefConfig, console, install);    
        compile = compile || install;
        generate = generate || compile;
        builder.Execute(generate, saveCppSource, compile, install);
    
}

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_CmdLine_CPP