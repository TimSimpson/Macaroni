#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_CPP

#include "Builder.h"
#include <boost/foreach.hpp>
#include <Macaroni/Build/BuilderArguments.h>
#include <Macaroni/Build/_.h>
#include <Macaroni/Environment/Console.h>
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/ContextPtr.h>
#include <Macaroni/Build/Cpp/CppFile.h>
#include <boost/filesystem/convenience.hpp>
#include <Macaroni/Exception.h>
#include <Macaroni/IO/FileSet.h>
#include <Macaroni/Build/GeneratorContext.h>
#include <Macaroni/Build/_.h>
#include <Macaroni/Build/_.h>
#include <Macaroni/Model/Library.h>
#include <Macaroni/Build/LibraryId.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/Environment/LuaEnvironment.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/ManifestOrganizer.h>
#include <Macaroni/Build/MCompiler.h>
#include <Macaroni/Build/MCompilerOptions.h>
#include <Macaroni/Parser/ParserException.h>
#include <boost/filesystem/operations.hpp>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/Paths.h>
#include <Macaroni/Environment/Process.h>
#include <boost/shared_ptr.hpp>
#include <Macaroni/Model/Source.h>
#include <string>
#include <sstream>
#include <vector>

using Macaroni::Build::BuilderArguments;
using Macaroni::Build::BuilderArgumentsPtr;
using Macaroni::Environment::Console;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Build::Cpp::CppFile;
using boost::filesystem::create_directories;
using Macaroni::Exception;
using Macaroni::IO::FileSet;
using Macaroni::Build::GeneratorContext;
// ~ <(I don't know how to generate a using statement for Macaroni::Build::InstallContext.) 
// ~ <(I don't know how to generate a using statement for Macaroni::Build::InstallContextPtr.) 
using Macaroni::Model::Library;
using Macaroni::Build::LibraryId;
using Macaroni::Model::LibraryPtr;
using Macaroni::Environment::LuaEnvironment;
using Macaroni::Build::Manifest;
using Macaroni::Build::ManifestPtr;
using Macaroni::Build::ManifestOrganizer;
using Macaroni::Build::MCompiler;
using Macaroni::Build::MCompilerOptions;
using Macaroni::Parser::ParserException;
using boost::filesystem::path;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::IO::Paths;
using Macaroni::Environment::Process;
using boost::shared_ptr;
using Macaroni::Model::Source;
using std::string;
using std::stringstream;
using std::vector;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

Builder::Builder(ContextPtr  context, path  manifestPath, Console &  console)
: compiler(), console(console), context(context), initialized(false), library(), manifest(), manifestPath(manifestPath), properties()
{
	        
    
}
path  Builder::climbToTop(std::vector<boost::filesystem::path > &  manifests, path  manifestPath)
{
	        
        manifests.push_back(manifestPath);
        path p = boost::filesystem::system_complete(manifestPath);
        if (p.has_parent_path())
        {            
            path mp = p.parent_path().parent_path() / "manifest.lua";        
            if (boost::filesystem::exists(mp))
            {
                return climbToTop(manifests, mp);
            }
        }
        return manifestPath;
    
}

        BuildContextPtr Builder::createBuildContext(ManifestPtr manifest, LibraryPtr library)
        {
            //const Path dir(manifest->GetRootDirectory(), 
            //               manifest->GetRootDirectory());
            vector<PathPtr> sources;
            BOOST_FOREACH(const std::string & srcDirectory, manifest->GetMSource())
            {                            
                path sd(srcDirectory);
                PathPtr newPath(new Path(sd)); //manifest->GetRootDirectory() / srcDirectory));
                sources.push_back(newPath);
            }
            path od(manifest->GetMOutput());
            PathPtr genSrc(new Path(od)); //manifest->GetRootDirectory() / manifest->GetMOutput()));
            path installPath = getInstallPath(manifest);
            PathPtr installPathPtr(new Path(installPath, installPath));
            BuildContextPtr iCon(new BuildContext(library, sources, genSrc, installPathPtr, properties));
            return iCon;
        }
    LibraryPtr  Builder::createLibraryForManifest(ManifestPtr  manifest)
{
	
        LibraryPtr newL(context->FindOrCreateLibrary(manifest->GetId()));
        return newL;
    
}
bool  Builder::Execute(const BuilderArguments &  arguments)
{
	        
        if (!initialized)
        {
            if (!Init())
            {
                console.WriteLine("An unrecoverable error occured in the initialization phase.");
                return false;
            }
        }
        return execute(arguments, manifest, library);
    
}
bool  Builder::execute(const BuilderArguments &  arguments, const ManifestPtr  manifest, const LibraryPtr  library)
{
	        
        console.WriteLine("--------------------------------------------------------------------------------");
        console.Write("    ");
        console.WriteLine(manifest->GetName());        
        console.WriteLine("--------------------------------------------------------------------------------");    

        //const Path dir(manifest->GetRootDirectory(), manifest->GetRootDirectory());
        //PathPtr mWork = dir.NewPathForceSlash("./MWork");
        //mWork->CreateDirectory();

        //PathPtr genSrc = dir.NewPathForceSlash(manifest->GetMOutput());//"/GeneratedSource");
        PathPtr genSrc(new Path(manifest->GetMOutput(), manifest->GetMOutput()));
        genSrc->CreateDirectory();

        bool success = parseChildren(arguments, manifest, library);        

        if (success && arguments.Generate())
        {
            success = ParseMacaroniSource(manifest, library);
            if (success)
            {
                BuildContextPtr iCon = createBuildContext(manifest, library);
                //GeneratorContextPtr gCon(new GeneratorContext(library, genSrc));
                success = manifest->RunTarget(console, iCon, "generate", "Generate")->Success;
            }
        }

        if (success && arguments.Compile())
        {
                        
            //const Path dir(manifest->GetRootDirectory(), manifest->GetRootDirectory());
            //PathPtr genSrc = dir.NewPathForceSlash(manifest->GetMOutput());//"/GeneratedSource");        
            //PathPtr src(new Path(manifest->GetRootDirectory(), manifest->GetRootDirectory()));
            //path installPath = getInstallPath();
            //PathPtr installPathPtr(new Path(installPath, installPath));
            BuildContextPtr iCon = createBuildContext(manifest, library);//new BuildContext(library, src, genSrc, installPathPtr));
            success = manifest->RunTarget(console, iCon, "build", "Build")->Success;
        }

            //createCppSrcRoots();
            //createCppFileList();

            ///*if (saveCppSource)
            //{
            //    success = SaveCppSource();
            //}*/
        if (success && arguments.Install())
        {                
            success = Install(manifest, library);
        }        

        if (success)
        {
            console.WriteLine(" ~ YOU WIN! ~ ");
        }
        else
        {
            console.WriteLine(" ~ GAME OVER ~ ");
        }
        return success;
    
}
path  Builder::getInstallPath(ManifestPtr  manifest)
{
	
        path userPath(Paths::GetUserPath());
        return userPath / "Libraries" / manifest->GetGroup()
                        / manifest->GetName() / manifest->GetVersion();        
    
}
bool  Builder::Init()
{
	        
        initialized = true;
        properties = RunInitialScript();
        vector<path> manifests;
        path topParent = climbToTop(manifests, manifestPath);
        for (int i = manifests.size() - 1; i >= 0; i --)
        {
            loadManifest(manifests[i], manifest, library);
            properties = manifest->GetProperties();
            if (!parseManifest(manifest, library))
            {
                return false;
            }
        }
        return true;
        // Finally, the manifest and library of whatever we're working on is loaded.
        // Now, get a list of the children...

    //    /*struct Resolve : public LocalManifestResolver 
    //    {
    //        virtual optional<LibraryId> FindParent(const optional<string &> & parentPath) 
    //        {
    //            bool mustExist = false;
    //            path filePath;
    //            if (!parentPath)
    //            {
    //                filePath = manifestPath.branch_path();                    
    //            }
    //            else
    //            {
    //                filePath = manifestPath / (parentPath.get());
    //                mustExist = true;
    //            }                
    //            if (!boost::filesystem::exists(filePath))
    //            {
    //                if (mustExist) 
    //                {
    //                    stringstream ss;
    //                    ss    << "Could not find parent manifest of \""
    //                        << manifestPath << "\" given the path \""
    //                        << parentPath << "\".";
    //                    throw Exception(ss.str().c_str());
    //                }
    //                return optional<LibraryId>();
    //            }

    //        }
    //
    //        virtual LibraryId FindChild(const std::string & childPath) = 0;

    //        virtual void SelfLibraryId(const LibraryId & myId) = 0;
    //    };        */        
    
}
bool  Builder::Install(ManifestPtr  manifest, LibraryPtr  library)
{
	        
        path installPath = getInstallPath(manifest);
        if (boost::filesystem::exists(installPath))
        {
            boost::filesystem::remove_all(installPath);
        }
        else
        {
            boost::filesystem::create_directories(installPath);
        }

        // Interface.mh - copy this file to root of directory.
        path localMhInterface(manifest->GetRootDirectory());
        localMhInterface = localMhInterface / manifest->GetMOutput();
        localMhInterface = localMhInterface / "Interface.mh";
        //path exportMhInterfaceDir(installPath);
        //exportMhInterfaceDir = exportMhInterfaceDir;
        //boost::filesystem::create_directories(exportMhInterfaceDir);
        //path exportMhInterface = exportMhInterfaceDir/ "Interface.mh";
        //boost::filesystem::copy_file(localMhInterface, exportMhInterface);

        //const Path dir(manifest->GetRootDirectory(), manifest->GetRootDirectory());
        //PathPtr genSrc = dir.NewPathForceSlash(manifest->GetMOutput());//"/GeneratedSource");        
        //PathPtr src(new Path(manifest->GetRootDirectory(), manifest->GetRootDirectory()));
        //PathPtr installPathPtr(new Path(installPath, installPath));
        BuildContextPtr iCon = createBuildContext(manifest, library); //(new BuildContext(library, src, genSrc, installPathPtr));
        Manifest::RunResultPtr result = manifest->RunTarget(console, iCon, "install", "Install");
        //Path::CopyDirectoryContents(headersLocal, headersInstall);
        // return true;
        if (result->Success) 
        {
            manifest->SaveAs(installPath / "manifest-final.lua", result->RunList);
        }
        return result->Success;
    
}
void  Builder::loadManifest(path  manifestPath, ManifestPtr &  outManifest, LibraryPtr &  outLibrary)
{
	
        ManifestPtr newM(new Manifest(manifestPath, properties));
        outManifest = newM;
        outLibrary = createLibraryForManifest(outManifest);
    
}
bool  Builder::parseChildren(const BuilderArguments &  arguments, const ManifestPtr  manifest, const LibraryPtr  library)
{
	
        std::vector<ManifestPtr> childManifests;
        BOOST_FOREACH(const string & child, manifest->GetChildren())
        {            
            path childPath(manifest->GetRootDirectory());
            childPath = childPath / child / "manifest.lua";
            ManifestPtr childManifest(new Manifest(childPath, properties));
            childManifests.push_back(childManifest);
        }
        ManifestOrganizer list(childManifests);
        list.Sort();
        BOOST_FOREACH(ManifestPtr manifest, childManifests)
        {
            // Create libraries so that the final-manifest will not be searched.
            createLibraryForManifest(manifest);            
        }
        BOOST_FOREACH(ManifestPtr manifest, childManifests)
        {
            LibraryPtr library = context->FindLibrary(manifest->GetId());
            parseManifest(manifest, library);
            if (!execute(arguments, manifest, library))
            {
                return false;
            }
        }                
        return true; 
    
}
void  Builder::parseDependencies(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, ManifestPtr  sourceManifest, LibraryPtr  sourceLibrary)
{
	
        BOOST_FOREACH(const LibraryId id, sourceManifest->GetDependencies()) 
        {            
            dependencyStack.push_back(id);
            parseDependency(dependencyStack, sourceManifest, sourceLibrary, id);
            dependencyStack.pop_back();
        }
    
}
void  Builder::parseDependency(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, ManifestPtr  sourceManifest, LibraryPtr  sourceLibrary, const LibraryId  dependencyId)
{
	
        LibraryPtr existingLib = sourceLibrary->GetContext()->FindLibrary(dependencyId);
        if (!existingLib)
        {            
            // Load manifest of dependency, create libraries
            ManifestPtr dManifest(new Manifest(dependencyId.FindFinalManifestFile(), properties));            
            LibraryPtr dLib = sourceLibrary->GetContext()->FindOrCreateLibrary(dependencyId);
            // Now parse all of the dependency's dependencies before continuing.
            parseDependencies(dependencyStack, dManifest, dLib);        
            // Now add all dependencies to library

            // Parse all of its source.
            parseManifestSource(dManifest, dLib);
            
            // Run prepare phase of dependency
            BuildContextPtr iCon = createBuildContext(dManifest, dLib);
            iCon->GetOutputDir()->CreateDirectory();
            //TODO: Don't really care if this fails.  But it probably shouldn't 
            // print a message to the console.
            Manifest::RunResultPtr result = dManifest->RunTarget(console, iCon, "prepare", "Prepare");

            // Now, add it as a dependency of this library.
            sourceLibrary->AddDependency(dLib.get());
            existingLib = dLib;
        }
        sourceLibrary->AddDependency(existingLib);
    
}
bool  Builder::ParseMacaroniSource(ManifestPtr  manifest, LibraryPtr  library)
{
	        
        console.WriteLine("~ Creating Macaroni model from source files (.mh, .mcpp). ~");
        try
        {
            parseManifestSource(manifest, library);
        }
        catch(Macaroni::Exception & ex)
        {
            console.Write("An error occured parsing Macaroni source files.");
            console.WriteLine(ex.GetSource());
            console.WriteLine(ex.GetMessage());
            return false;
        }
        catch(Macaroni::Parser::ParserException & pe)
        {
            console.Write("An error occured parsing Macaroni source files.");
            console.WriteLine(pe.GetSource()->ToString());
            console.WriteLine(pe.GetMessage());
            return false;
        }
    
}
bool  Builder::parseManifest(ManifestPtr &  manifest, LibraryPtr &  library)
{
	
        vector<LibraryId> dependencyStack;
        dependencyStack.push_back(manifest->GetId());
        try 
        {
            ///*stringstream ss;
            //ss << "~ Parsing dependencies for {[["
            //    << library->GetGroup() 
            //    << "]], [[" << library->GetName()
            //    << "]], [[" << library->GetVersion() 
            //    << "]]}";
            //console.WriteLine(ss.str().c_str());*/
            parseDependencies(dependencyStack, manifest, library);
            //parseNestedLibraries(dependencyStack, manifest, library);
            //console.WriteLine("~ Creating Macaroni model from source files (.mh, .mcpp). ~");
            //parseManifestSource(manifest, library);
        }
        catch(Macaroni::Exception & ex)
        {
            console.Write("An error occured in ");
            printOutDependencyStack(dependencyStack);
            console.WriteLine(".");
            console.WriteLine(ex.GetSource());
            console.WriteLine(ex.GetMessage());
            return false;
        }
        catch(Macaroni::Parser::ParserException & pe)
        {
            console.Write("An error occured parsing Macaroni code while in dependency ");
            printOutDependencyStack(dependencyStack);
            console.WriteLine(".");
            console.WriteLine(pe.GetSource()->ToString());
            console.WriteLine(pe.GetMessage());
            return false;
        }
        catch(std::exception & ex)
        {
            console.Write("An error occured in ");
            printOutDependencyStack(dependencyStack);
            console.WriteLine(".");
            console.WriteLine(ex.what());
            return false;
        }
        return true;
    /*
        


        std::string mSrc = manifest->GetMSource()[0];
        std::string mOut = manifest->GetMOutput();
        std::vector<FileSet> inputFiles;

        // Include dependencies
        BOOST_FOREACH(ManifestPtr dependency, manifest->GetDependencies()) 
        {
            parseDependency(library, compiler, dependency);
        }
        // end include dependencies

        inputFiles.push_back(FileSet(boost::filesystem::path(mSrc), "\\.mcpp$"));

        std::vector<const std::string> blankGeneratorSelection;
        MCompilerOptions options(inputFiles,
                                 boost::filesystem::path(mOut),
                                 blankGeneratorSelection);

        try
        {
            compiler.Compile(library, options);
        }
        catch(Macaroni::Exception & ex)
        {
            console.WriteLine("An error occured during Macaroni phase.");
            console.WriteLine(ex.GetSource());
            console.WriteLine(ex.GetMessage());
            return false;
        }
        catch(Macaroni::Parser::ParserException & pe)
        {
            console.WriteLine("Error parsing Macaroni code: ");
            console.WriteLine(pe.GetSource()->ToString());
            console.WriteLine(pe.GetMessage());
            return false;
        }
        return true;*/        
    
}
void  Builder::parseManifestSource(ManifestPtr  sourceManifest, LibraryPtr  sourceLibrary)
{
	
        const std::vector<const std::string> src = sourceManifest->GetMSource();
        BOOST_FOREACH(const string & dir, src)
        {            
            std::vector<FileSet> input;
            input.push_back(FileSet(boost::filesystem::path(dir), "\\.m(cpp|h)?$"));            
            compiler.BuildModel(sourceLibrary, input);            
        }
    
}
void  Builder::parseNestedLibraries(std::vector<Macaroni::Build::LibraryId > &  dependencyStack, ManifestPtr  sourceManifest, LibraryPtr  sourceLibrary)
{
	
        BOOST_FOREACH(const string & childDir, sourceManifest->GetChildren())
        {

        }
    
}
void  Builder::printOutDependencyStack(std::vector<Macaroni::Build::LibraryId > &  dependencyStack)
{
	
        bool seenOne = false;
        BOOST_FOREACH(LibraryId & id, dependencyStack)
        {
            if (!seenOne) 
            {
                seenOne = true;
            }
            else 
            {
                console.Write(" -> ");
            }
            console.Write(id.GetGroup());
            console.Write("|");
            console.Write(id.GetName());
            console.Write("|");
            console.Write(id.GetVersion());
        }
    
}
string  Builder::RunInitialScript()
{
	
        path userPath(Paths::GetUserPath());
        path initLuaPath = userPath / "init.lua";        
        if (!boost::filesystem::exists(initLuaPath))
        {
            stringstream ss;
            ss    << "Could not find init.lua at "
                << initLuaPath 
                << ".";
            console.WriteLine(ss.str());
            return "{}";            
        } 
        else 
        {
            LuaEnvironment env;
            env.ParseFile(initLuaPath.string());
            env.Run();
            stringstream cereal;
            env.SerializeTable(cereal, "properties");
            return cereal.str();
        }        
    
}

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_Builder_CPP