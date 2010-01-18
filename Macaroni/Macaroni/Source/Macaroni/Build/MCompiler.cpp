#ifndef MACARONI_BUILD_COMPILER_CPP
#define MACARONI_BUILD_COMPILER_CPP

#include "MCompilerOptions.h"
#include "../Model/Context.h"
#include "../Generator/Cpp/CppSourceGenerator.h"
#include "../Generator/DebugEnumerator.h"
#include "../Exception.h"
#include "../Model/FileName.h"
#include "../Model/Library.lh"
#include <memory>
#include "../Model/Node.h"
#include "../Model/MemberVisitor.h"
#include "../Parser/ParserException.h"
#include "../Parser/Pippy/PippyParser.h"
#include "../Generator/DynamicGenerators.h"
#include "../Model/Source.h"

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Generator::Cpp::CppSourceGenerator;
using Macaroni::Generator::DebugEnumerator;
using boost::filesystem::directory_iterator;
using Macaroni::Exception;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::IO::FileSet;
#include <fstream>
#include <iostream>
using Macaroni::Model::Library;
using Macaroni::Model::LibraryPtr;
using Macaroni::Model::MemberVisitor;
using Macaroni::Parser::ParserException;
using boost::filesystem::path;
using Macaroni::Parser::Pippy::PippyParser;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
#include <string>
#include <sstream>
#include <vector>


namespace Macaroni { namespace Build {

class MCompiler
{
public:
	/** Iteratres all input files, parsing each one into the given context. */
	bool BuildModel(LibraryPtr library, const std::vector<FileSet> filePath);
	void Compile(LibraryPtr library,
				 const MCompilerOptions & options);

private:	
	/** Reads from the model to generates output files. */
	bool generateFiles(LibraryPtr library, path output, const MCompilerOptions & options);
	/** Parses the file and stores it into the Model context. */
	void parseFile(LibraryPtr library, path filePath);
	bool parseFileSet(LibraryPtr library, FileSet files);
	/** Reads the contents of a file into the contents stringstream. */
	void readFile(std::stringstream & contents, const std::string & filePath);
};


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
	
	///*std::cout << "Debug Tree\n";
	//DebugEnumerator de;
	//de.Iterate(context, std::cout);*/

	/*
	std::cout << "Generating output...";
 
	CppSourceGenerator sourceGen(output, 4);
	std::auto_ptr<MemberVisitor> visitor(sourceGen.CreateRootVisitor());
	if (!!context->GetRoot()->GetMember())
	{
		context->GetRoot()->GetMember()->Visit(visitor.get());
	}*/

	for(unsigned int i = 0; i < options.GetGenerators().size(); i ++)
	{
		boost::filesystem::path genPath =
			Generator::ResolveGeneratorPath(options.GetInput(), options.GetGenerators()[i]);
		if (!genPath.empty())
		{
			Generator::RunDynamicGenerator(library, output, genPath);
		}
	}
	//Generator::RunDynamicGenerators(context, output);

	return true;
}

void MCompiler::Compile(LibraryPtr library, const MCompilerOptions & options)
{
	/*std::cout << "Macaroni for C++\n";
	std::cout << "(C) Tim Simpson\n";
	std::cout << "Version 0.0.1, June 22, 2009\n";
	std::cout << "\n";*/
//	std::cout << 
//"Compiler options-------------------------------------------------------------\n"
//<< "|\tInput:" << options.GetInput().GetRoot().string().c_str() << "\n"
//	<< "|\tOutput:" << options.GetOutput().string().c_str() << "\n"
//	<< 
//" ----------------------------------------------------------------------------\n"
//	<< "\n";	
	
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
