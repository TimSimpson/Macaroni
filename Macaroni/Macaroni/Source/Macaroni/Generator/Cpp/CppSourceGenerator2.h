#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_H
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_H

#include "../../ME.h"
#include "../../Model/Context.h"
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Generator, Cpp)

class CppSourceGenerator2
{
public:
	CppSourceGenerator2(Model::ContextPtr context, const boost::filesystem::path & path);
	
	~CppSourceGenerator2();
	
	void Run();

private:
	Model::ContextPtr context;

	boost::filesystem::path rootPath;

};

END_NAMESPACE

#endif

