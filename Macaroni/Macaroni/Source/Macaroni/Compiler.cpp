#ifndef MACARONI_COMPILER_CPP
#define MACARONI_COMPILER_CPP

#include "Compiler.h"
#include "Model/Context.h"
#include "Generator/Cpp/CppSourceGenerator.h"
#include "Generator/DebugEnumerator.h"
#include "Exception.h"
#include "Model/FileName.h"
#include <memory>
#include "Model/Node.h"
#include "Model/MemberVisitor.h"
#include "Parser/ParserException.h"
#include "Parser/Pippy/PippyParser.h"
#include "Model/Source.h"

using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using Macaroni::Generator::Cpp::CppSourceGenerator;
using Macaroni::Generator::DebugEnumerator;
using boost::filesystem::directory_iterator;
using Macaroni::Exception;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Gestalt::FileSystem::FileSet;
#include <fstream>
#include <iostream>
using Macaroni::Model::MemberVisitor;
using Macaroni::Parser::ParserException;
using boost::filesystem::path;
using Macaroni::Parser::Pippy::PippyParser;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
#include <string>
#include <sstream>

namespace Macaroni {

namespace Compiler
{

static void readFile(std::stringstream & contents, const std::string & filePath)
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

static void parseFile(ContextPtr context, path filePath)
{
	std::stringstream fileContents;
	readFile(fileContents, filePath.string());
	
	FileNamePtr fileName = FileName::Create(filePath.string());
	SourcePtr source = Source::Create(fileName, 1, 1);
	PippyParser parser;
	parser.Read(context, source, fileContents.str());
}

static bool buildModel(ContextPtr context, FileSet inputFiles)
{	
	std::cout << "Builing Macaroni::Model...\n";

	FileSet::Iterator itr = inputFiles.Begin();
	FileSet::Iterator end = inputFiles.End();
	for(; itr != end; ++ itr)
	{	
		path p = *itr;
		std::cout << p.string() << "\n";
		try
		{
			parseFile(context, p);
		} 
		catch(ParserException pe)
		{
			std::cerr << "ERROR: ";
			std::cerr << pe.GetSource()->ToString() << "\n";
			std::cerr << pe.GetMessage() << "\n";			
			return false;
		}
	}
	return true;
}

static bool generateFiles(ContextPtr context, path output)
{
	std::cout << "Debug Tree\n";
	DebugEnumerator de;
	de.Iterate(context, std::cout);

	std::cout << "Generating output...";

	CppSourceGenerator sourceGen(output, 4);
	std::auto_ptr<MemberVisitor> visitor(sourceGen.CreateRootVisitor());
	if (!!context->GetRoot()->GetMember())
	{
		context->GetRoot()->GetMember()->Visit(visitor.get());
	}

	return true;
}

void Compile(const CompilerOptions & options)
{
	std::cout << "Macaroni for C++\n";
	std::cout << "(C) Tim Simpson\n";
	std::cout << "Version 0.0.1, June 22, 2009\n";
	std::cout << "\n";

	ContextPtr context(new Context(std::string("%ROOT%")));
	
	if (!buildModel(context, options.GetInput()))
	{
		std::cerr << "GAME OVER\n";
		return;
	}

	if (!generateFiles(context, options.GetOutput()))
	{
			std::cerr << "GAME OVER\n";
		return;
	}

	std::cerr << "YOU WIN!\n";

}

};


}

#endif
