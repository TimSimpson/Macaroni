#ifndef MACARONI_BUILD_COMPILER_CPP
#define MACARONI_BUILD_COMPILER_CPP

#include "MCompilerOptions.h"
#include "../Model/Context.h"
//#include "../Generator/Cpp/CppSourceGenerator.h"
#include "../Generator/DebugEnumerator.h"
#include "../Exception.h"
#include <Macaroni/Model/FileName.h>
#include <Macaroni/Build/BuildContext.h>
#include "../Model/LibraryPtr.h"
#include <memory>
#include "../Model/Node.h"
#include "../Model/MemberVisitor.h"
#include "../Parser/ParserException.h"
#include "../Parser/Pippy/PippyParser.h"
#include <Macaroni/Generator/DynamicGeneratorRunner.h>
#include <Macaroni/IO/Path.h>
#include "../Model/Source.h"
#include <Macaroni/Environment/StringPair.h>

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
//using Macaroni::Generator::Cpp::CppSourceGenerator;
using Macaroni::Generator::DebugEnumerator;
using boost::filesystem::directory_iterator;
using Macaroni::Generator::DynamicGeneratorRunner;
using Macaroni::Exception;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::IO::FileSet;
#include <fstream>
#include <iostream>
using Macaroni::Build::BuildContext;
using Macaroni::Build::BuildContextPtr;
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Model::MemberVisitor;
using Macaroni::Parser::ParserException;
using boost::filesystem::path;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;
using Macaroni::Parser::Pippy::PippyParser;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
#include <string>
using Macaroni::Environment::StringPair;
#include <sstream>
#include <vector>


namespace Macaroni { namespace Build {

class MCompiler
{
public:
	MCompiler(const Macaroni::AppPathsPtr & appPaths);
	/** Iteratres all input files, parsing each one into the given context. */
	bool BuildModel(LibraryPtr library, const std::vector<FileSet> filePath);
	void Compile(LibraryPtr library,
				 const MCompilerOptions & options);

private:	
	AppPathsPtr appPaths;

	/** Reads from the model to generates output files. */
	bool generateFiles(LibraryPtr library, path output, const MCompilerOptions & options);
	/** Parses the file and stores it into the Model context. */
	void parseFile(LibraryPtr library, path filePath);
	bool parseFileSet(LibraryPtr library, FileSet files);
	/** Reads the contents of a file into the contents stringstream. */
	void readFile(std::stringstream & contents, const std::string & filePath);
};


MCompiler::MCompiler(const Macaroni::AppPathsPtr & appPaths)
:appPaths(appPaths)
{
}

void MCompiler::readFile(std::stringstream & contents, const std::string & filePath)
{
	std::ifstream file(filePath.c_str());
	if (!file.is_open())
	{
		std::stringstream ss;
		ss << "Could not open file " << filePath << "!";
		throw Macaroni::Exception(ss.str().c_str());
	}	
	std::string line;
	while(! file.eof())
	{
		std::getline(file, line);
		contents << line << "\n";
	}
	file.close();
}

void MCompiler::parseFile(LibraryPtr library, path filePath)
{
	std::stringstream fileContents;
	readFile(fileContents, filePath.string());
	
	FileNamePtr fileName = FileName::Create(filePath.string());
	SourcePtr source = Source::Create(fileName, 1, 1);
	PippyParser parser;
	parser.Read(library, source, fileContents.str());
}

bool MCompiler::BuildModel(LibraryPtr library, const std::vector<FileSet> inputFiles)
{	
	std::cout << "Builing Macaroni::Model...\n";
	for (unsigned int i = 0; i < inputFiles.size(); i ++)
	{
		const FileSet & files = inputFiles[i];
		if (!parseFileSet(library, files)) 
		{
			return false;
		}
	}	
	return true;
}



bool MCompiler::generateFiles(LibraryPtr library, path output, 
							  const MCompilerOptions & options)
{
	for(unsigned int i = 0; i < options.GetGenerators().size(); i ++)
	{
		DynamicGeneratorRunner runner(appPaths);
		boost::filesystem::path genPath =
			runner.ResolveGeneratorPathOLD(options.GetInput(), options.GetGenerators()[i]);
		if (!genPath.empty())
		{
			PathPtr outputPath(new Path(output, output));
			std::vector<PathPtr> sources;
			PathPtr installDir; // nullptr
			BuildContextPtr buildContext(new BuildContext(appPaths, library, sources, outputPath, installDir, "{}"));
			runner.RunDynamicGenerator(genPath, buildContext, "Generate", "{}");
		}
	} 
	
	return true;
}

void MCompiler::Compile(LibraryPtr library, const MCompilerOptions & options)
{		
	if (!BuildModel(library, options.GetInput()))
	{
		std::cerr << "GAME OVER\n";
		return;
	}

	if (!generateFiles(library, options.GetOutput(), options))
	{
			std::cerr << "GAME OVER\n";
		return;
	}

	//std::cerr << "YOU WIN!\n";

}

bool MCompiler::parseFileSet(LibraryPtr library, const FileSet inputFiles)
{	
	FileSet::Iterator itr = inputFiles.Begin();
	FileSet::Iterator end = inputFiles.End();
	for(; itr != end; ++ itr)
	{	
		path p = *itr;
		std::cout << p.string() << "\n";
		/*try
		{*/
			parseFile(library, p);
		/*} 
		catch(ParserException pe)
		{
			std::cerr << "ERROR: ";
			std::cerr << pe.GetSource()->ToString() << "\n";
			std::cerr << pe.GetMessage() << "\n";			
			return false;
		}*/
	}
	return true;
}

} }

#endif
