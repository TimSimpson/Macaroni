/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_BUILD_COMPILER_CPP
#define MACARONI_BUILD_COMPILER_CPP

#include "MCompilerOptions.h"
#include <Macaroni/Model/Context.h>
//#include "../Generator/Cpp/CppSourceGenerator.h"
#include "../Generator/DebugEnumerator.h"
#include <Macaroni/Exception.h>
#include <Macaroni/Model/FileName.h>
#include <Macaroni/IO/FileSet.h>
#include <Macaroni/Build/BuildContext.h>
#include "../Model/LibraryPtr.h"
#include <memory>
#include "../Model/Node.h"
#include "../Model/MemberVisitor.h"
#include <Macaroni/Parser/ParserException.h>
#include "../Parser/Pippy/PippyParser.h"
#include <Macaroni/Generator/DynamicGeneratorRunner.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/IO/RegexFileSet.h>
#include <Macaroni/IO/RegexFileSetIterator.h>
#include <Macaroni/Model/Source.h>
#include <Macaroni/StringException.h>
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
using Macaroni::IO::RegexFileSet;
using Macaroni::IO::RegexFileSetIterator;
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
	bool BuildModel(LibraryPtr library, const std::vector<RegexFileSet> & filePath);
	void Compile(LibraryPtr library,
				 const MCompilerOptions & options);

private:
	AppPathsPtr appPaths;

	/** Reads from the model to generates output files. */
	bool generateFiles(LibraryPtr library, path output, const MCompilerOptions & options);
	/** Parses the file and stores it into the Model context. */
	void parseFile(LibraryPtr library, Macaroni::IO::Path filePath);
	bool parseFileSet(LibraryPtr library, RegexFileSet files);
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
		throw Macaroni::StringException(ss.str().c_str());
	}
	std::string line;
	while(! file.eof())
	{
		std::getline(file, line);
		contents << line << "\n";
	}
	file.close();
}

void MCompiler::parseFile(LibraryPtr library, Macaroni::IO::Path filePath)
{
	std::stringstream fileContents;
	readFile(fileContents, filePath.GetAbsolutePath());

	FileNamePtr fileName = FileName::Create(filePath);
	SourcePtr source = Source::Create(fileName, 1, 1);
	PippyParser parser;
	parser.Read(library, source, fileContents.str());
}

bool MCompiler::BuildModel(LibraryPtr library, 
						   const std::vector<RegexFileSet> & inputFiles)
{
	for (unsigned int i = 0; i < inputFiles.size(); i ++)
	{
		const RegexFileSet & files = inputFiles[i];
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

bool MCompiler::parseFileSet(LibraryPtr library, const RegexFileSet inputFiles)
{
	RegexFileSetIterator itr = inputFiles.Begin();
	RegexFileSetIterator end = inputFiles.End();
	for(; itr != end; ++ itr)
	{
		path p = *itr;
		/*try
		{*/
		Macaroni::IO::Path relativePath(inputFiles.GetRoot(), p);
			parseFile(library, relativePath);
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
