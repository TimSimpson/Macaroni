#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_H
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_H

#include "../../ME.h"
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Generator, Cpp)

class CppSourceGenerator 
{
public:
	CppSourceGenerator(const boost::filesystem::path & path, int tabCount);
	
	void CreateFileNameForNode(Macaroni::Model::NodePtr node, 
							   std::string & hFilePath, 
							   std::string & cppFilePath);

	Model::MemberVisitor * CreateRootVisitor();

	const boost::filesystem::path & GetRootPath() const;

	int GetTabCount();

private:

	// These are the streams used for the global namespace.
	std::ofstream cppFile;

	std::ofstream hFile;

	int tabCount;

	boost::filesystem::path rootPath;
};

END_NAMESPACE

#endif

