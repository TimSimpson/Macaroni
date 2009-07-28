#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_CPP
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_CPP

#include "CppSourceGenerator.h"
#include "../../Model/Cpp/Class.h"
#include <fstream>
#include "../../Model/Cpp/Function.h"
#include <iostream>
#include "../../Model/Node.h"
#include "../../Model/MemberVisitor.h"
#include "../../Model/Cpp/Namespace.h"
#include <string>
#include <sstream>
#include "../../Model/Cpp/TypeInfo.h"
#include "../../Model/Cpp/Variable.h"

using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Member;
using Macaroni::Model::MemberPtr;
using Macaroni::Model::MemberVisitor;
using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Cpp::NamespacePtr;
using Macaroni::Model::Node;
using Macaroni::Model::NodePtr;
using Macaroni::Model::NodeList;
using Macaroni::Model::NodeListPtr;
using boost::filesystem::path;
using Macaroni::Model::Cpp::TypeInfo;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariablePtr;

BEGIN_NAMESPACE(Macaroni, Generator, Cpp)

class ScopeVisitor : public MemberVisitor
{
public:

	ScopeVisitor(CppSourceGenerator * parent, std::ofstream & hFile, std::ofstream & cppFile, int depth)
		: parent(parent),
		  hFile(hFile),
		  cppFile(cppFile),
		  depth(depth)
	{
	}

	virtual MemberVisitor * VisitClass(const Macaroni::Model::Cpp::Class &);

	virtual MemberVisitor * VisitNamespace(const Macaroni::Model::Cpp::Namespace &);
		
	virtual void VisitFunction(const Macaroni::Model::Cpp::Function & func)
	{
		startLine();

		writeTypeInfo(func.GetReturnType());
		//write(" ");
		NodePtr parent = func.GetNode()->GetNode();
		MemberPtr parentMember = parent->GetMember();
		if (boost::dynamic_pointer_cast<Class>(parentMember))
		{
			ClassPtr parentClass = boost::dynamic_pointer_cast<Class>(parentMember);
			cppFile << parentClass->GetName() << "::";
		}
		write(func.GetName());
		if (func.GetArgumentCount() == 0)
		{
			write("()");
		}
		else
		{
			startLine();
			write("(");		
			depth ++;		
			startLine();
			for(int i = 0; i < func.GetArgumentCount(); i ++)
			{
				if (i > 0)
				{
					write(", ");
					startLine();
				}
				VariablePtr arg = func.GetArgument(i);
				writeTypeInfo(arg->GetTypeInfo());
				//write(" ");
				write(arg->GetName());			
			}
			depth --;
			startLine();
			write(")");
		}
		hFile << ";";

		startLineCppFile();
		cppFile << "{";
		depth ++;
		//startLineCppFile();
		cppFile << func.GetCodeBlock();
		depth --;
		startLineCppFile();
		cppFile << "}";
		startLineCppFile();
	}

	virtual void VisitVariable(const Macaroni::Model::Cpp::Variable & var)
	{		
		writeTypeInfoHFile(var.GetTypeInfo());

		hFile << var.GetName() << ";";
		//cppFile << var.GetName() << ";";
	}

protected:	

	std::ofstream & cppFile;

	int depth;

	std::ofstream & hFile;

	boost::filesystem::path rootPath;

	void startLineCppFile()
	{
		cppFile << "\n" << std::string(depth * parent->GetTabCount(), ' ');
	}

	void startLineHFile()
	{
		hFile   << "\n" << std::string(depth * parent->GetTabCount(), ' ');
	}

	void startLine()
	{
		startLineHFile();
		startLineCppFile();
	}	

	void write(const char * msg)
	{
		hFile << msg;
		cppFile << msg;
	}

	void write(const std::string & msg)
	{
		hFile << msg;
		cppFile << msg;
	}

	void writeFooter(const Member & m)
	{
		startLine();
		startLine();

		NamespacePtr ns = m.FindClosestParentNamespace();

		std::vector<std::string> subNames;
		Node::SplitComplexName(ns->GetNode()->GetPrettyFullName("::"), subNames);
		for(size_t i = 0; i < subNames.size(); i ++)
		{
			write("} ");		
		}
		write("// End namespace ");
		write(ns->GetNode()->GetPrettyFullName("::"));
		startLine();
		startLine();
		write("#endif // End Macaroni compile guard.");
		startLine();
	}

	void writeHeaderCompileGuard(const Member & m)
	{
		hFile << "#ifndef MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_H\n";
		hFile << "#define MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_H\n";
		cppFile << "#ifndef MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_CPP\n";
		cppFile << "#define MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_CPP\n";
		
		startLine();
	}

	void writeHeaderIncludeStatements(const Member & m, NodeListPtr imports)
	{		
		for(unsigned int i = 0; i < imports->size(); i ++)
		{
			NodePtr import = (*imports)[i];
			std::ofstream & file = 
				(m.DoesDefinitionReference(import)) ? hFile : cppFile;			
			file << "import \"" << import->GetPrettyFullName("/") << ".h\"";
		}
	}

	void writeHeaderUsingStatements(NodeListPtr imports)
	{
		for(unsigned int i = 0; i < imports->size(); i ++)
		{
			NodePtr import = (*imports)[i];					
			cppFile << "using " << import->GetPrettyFullName("::") << ";";
		}
	}

	void writeHeaderNamespaceStart(const Member & n)
	{
		NamespacePtr ns = n.FindClosestParentNamespace();

		std::vector<std::string> subNames;
		Node::SplitComplexName(ns->GetNode()->GetPrettyFullName("::"), subNames);
		for(size_t i = 0; i < subNames.size(); i ++)
		{
			write("namespace ");
			write(subNames[i]);
			write("{ ");			
		}
		startLine();
		startLine();
	}

	

	void writeTypeInfo(const TypeInfo & type)
	{
		if (type.IsConst)
		{
			write("const ");			
		}
		hFile << type.Node->GetPrettyFullName("::") <<  " ";
		cppFile << type.Node->GetPrettyFullName("::") << " ";
		if (type.IsPointer)
		{
			write("* ");			
			if (type.IsConstPointer)
			{
				write("const ");				
			}
		}
		if (type.IsReference)
		{
			write("& ");			
		}		
	}

	void writeTypeInfoHFile(const TypeInfo & type)
	{
		if (type.IsConst)
		{
			hFile << "const ";			
		}
		hFile << type.Node->GetPrettyFullName("::") <<  " ";
		if (type.IsPointer)
		{
			hFile << "* ";			
			if (type.IsConstPointer)
			{
				hFile << "const ";				
			}
		}
		if (type.IsReference)
		{
			hFile << "& ";			
		}		
	}

private:

	CppSourceGenerator * parent;
	
};

class NamespaceVisitor : public ScopeVisitor
{
public:
	NamespaceVisitor(CppSourceGenerator * parent, 
					 std::ofstream & hFile, std::ofstream & cppFile, 
					 int depth, const Namespace & ns)
	: ScopeVisitor(parent, hFile, cppFile, depth), fileNamespace(ns)
	{
		writeHeaderCompileGuard(fileNamespace);
		writeHeaderNamespaceStart(fileNamespace);
	}

	~NamespaceVisitor()
	{
		writeFooter(fileNamespace);
		delete &hFile;
		delete &cppFile;
	}
private:
	const Namespace & fileNamespace;
};


MemberVisitor * ScopeVisitor::VisitNamespace(const Macaroni::Model::Cpp::Namespace & ns)
{
	std::auto_ptr<std::ofstream> hFilePtr(new std::ofstream);
	std::auto_ptr<std::ofstream> cppFilePtr(new std::ofstream);

	if (ns.GetNode()->GetNode() != nullptr)
	{
		path dirPath(parent->GetRootPath() / path("/")) 
			/ path(ns.GetNode()->GetNode()->GetPrettyFullName("/")));
		boost::filesystem::create_directory(dirPath);
	}

	std::stringstream ss;
	ss << parent->GetRootPath().string() << "/" << ns.GetNode()->GetPrettyFullName("/") << "/global.h";	
	hFilePtr->open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(hFilePtr->is_open(), "HFile not opened.");

	std::string p = ss.str();

	ss.str("");
	ss << parent->GetRootPath().string() << "/" << ns.GetNode()->GetPrettyFullName("/") << "/global.cpp";
	cppFilePtr->open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(cppFilePtr->is_open(), "CppFile not opened.");
	
	NamespaceVisitor * nsv = new NamespaceVisitor(parent, 
		dynamic_cast<std::ofstream &>(*hFilePtr.get()),
		dynamic_cast<std::ofstream &>(*cppFilePtr.get()),
		0,
		ns);
	hFilePtr.release();
	cppFilePtr.release();
	return nsv;
}


class ClassVisitor : public ScopeVisitor
{
public:
	ClassVisitor(CppSourceGenerator * parent, 
					 std::ofstream & hFile, std::ofstream & cppFile, 
					 int depth, const Class & fileClass)
	: ScopeVisitor(parent, hFile, cppFile, depth), fileClass(fileClass)
	{
		writeHeaderCompileGuard(fileClass);
		writeHeaderIncludeStatements(fileClass, fileClass.GetImportedNodes());
		writeHeaderUsingStatements(fileClass.GetImportedNodes());
		writeHeaderNamespaceStart(fileClass);

		hFile << "class " << fileClass.GetName() << "\n";
		hFile << "{\n";	
		//depth ++;
	}

	~ClassVisitor()
	{
		//depth --;	
		hFile << "\n";
		hFile << "} // End of class " << fileClass.GetName() << "\n";
		writeFooter(fileClass);

		delete &hFile;
		delete &cppFile;
	}
private:
	const Class & fileClass;
};


MemberVisitor * ScopeVisitor::VisitClass(const Macaroni::Model::Cpp::Class & c)
{
	std::auto_ptr<std::ofstream> hFilePtr(new std::ofstream);
	std::auto_ptr<std::ofstream> cppFilePtr(new std::ofstream);

	if (c.GetNode()->GetNode() != nullptr)
	{
		path dirPath(parent->GetRootPath() / path("/") 
			/ path(c.GetNode()->GetNode()->GetPrettyFullName("/")) );
		boost::filesystem::create_directory(dirPath);
	}

	std::stringstream ss;
	ss << parent->GetRootPath().string() << "/" << c.GetNode()->GetPrettyFullName("/") << ".h";	
	hFilePtr->open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(hFilePtr->is_open(), "HFile not opened.");

	std::string p = ss.str();

	ss.str("");
	ss << parent->GetRootPath().string() << "/" << c.GetNode()->GetPrettyFullName("/") << ".cpp";
	cppFilePtr->open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(cppFilePtr->is_open(), "CppFile not opened.");
	
	ClassVisitor * nsv = new ClassVisitor(parent, 
		dynamic_cast<std::ofstream &>(*hFilePtr.get()),
		dynamic_cast<std::ofstream &>(*cppFilePtr.get()),
		0,
		c
		);
	hFilePtr.release();
	cppFilePtr.release();
	return nsv;
}


CppSourceGenerator::CppSourceGenerator(const path & path, int tabCount)
:cppFile(), hFile(), rootPath(path), tabCount(tabCount)
{
	
}

void CppSourceGenerator::CreateFileNameForNode(NodePtr node,
											   std::string & hFilePath, 
											   std::string & cppFilePath)
{
	std::stringstream ss;
	ss << rootPath.string() << "/" << node->GetPrettyFullName("/")
	   << ".h";
	hFilePath = ss.str();
	
	ss.str("");
	ss << rootPath.string() << "/" << node->GetPrettyFullName("/")
	   << ".cpp";
	cppFilePath = ss.str();
}

MemberVisitor * CppSourceGenerator::CreateRootVisitor()
{
	class RootVisitor : public ScopeVisitor
	{
	public:
		RootVisitor(CppSourceGenerator * parent, 
					std::ofstream & hFile, std::ofstream & cppFile, 
					int depth)
		: ScopeVisitor(parent, hFile, cppFile, depth)
		{
		}
		~RootVisitor()
		{
			hFile.close();
			cppFile.close();
		}
	};

	
	std::stringstream ss;
	ss << rootPath.string() << "/global.h";
	hFile.open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(hFile.is_open(), "HFile not opened.");

	std::string s = ss.str();

	ss.str("");
	ss << rootPath.string() << "/global.cpp";
	cppFile.open(ss.str().c_str(), std::ios::out);
	MACARONI_ASSERT(cppFile.is_open(), "HFile not opened.");

	return new RootVisitor(this, hFile, cppFile, 0);
}

const boost::filesystem::path & CppSourceGenerator::GetRootPath() const
{
	return rootPath;
}

int CppSourceGenerator::GetTabCount()
{
	return tabCount;
}


END_NAMESPACE

#endif

