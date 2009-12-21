#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_CPP
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR_CPP

#include "CppSourceGenerator.h"
#include "../../Model/Cpp/Class.h"
#include "../../Model/Cpp/Constructor.h"
#include "../../Model/Cpp/Destructor.h"
#include <fstream>
#include "../../Model/Cpp/Function.h"
#include <iostream>
#include "../../Model/Node.h"
#include "../../Model/MemberVisitor.h"
#include "../../Model/Cpp/Namespace.h"
#include "../../Model/Cpp/Primitive.h"
#include <string>
#include <sstream>
#include "../../Model/Type.h"
#include "../../Model/Cpp/Variable.h"
#include "../../Model/Cpp/VariableAssignment.h"

using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr;
using Macaroni::Model::Cpp::Constructor;
using Macaroni::Model::Cpp::ConstructorPtr;
using Macaroni::Model::Cpp::Destructor;
using Macaroni::Model::Cpp::DestructorPtr;
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
using Macaroni::Model::Cpp::Primitive;
using Macaroni::Model::Type;
using Macaroni::Model::TypePtr;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariableAssignment;
using Macaroni::Model::Cpp::VariablePtr;

#define MACARONI_OVERWRITE_ALLOWED "//~ MACARONI_FILE_OVERWRITE_ALLOWED"

BEGIN_NAMESPACE(Macaroni, Generator, Cpp)

namespace
{
	bool isOkToOverwriteFile(const char * filePath)
	{
		if (!boost::filesystem::exists(filePath))
		{
			return true;
		}
		std::ifstream input(filePath);		
		std::string firstLine;
		std::getline(input, firstLine);
		input.close();
		return (firstLine == MACARONI_OVERWRITE_ALLOWED);
	}

	void openFileIfPossible(std::ofstream & outputFile, const char * filePath)
	{
		if (isOkToOverwriteFile(filePath))
		{
			outputFile.open(filePath, std::ios::out);
			std::stringstream ss;
			ss << "Could not open file \"" << filePath << "\".";
			MACARONI_ASSERT(outputFile.is_open(), ss.str().c_str());
			outputFile << std::string(MACARONI_OVERWRITE_ALLOWED) << "\n";
			std::cout << "Opening file " << std::string(filePath) << "...\n";
		}
		else
		{
			std::cout << "Skipping file " << std::string(filePath) << "...\n";			
		}
	}
}

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

	virtual void VisitClassFooter()
	{
	}

	virtual MemberVisitor * VisitNamespace(const Macaroni::Model::Cpp::Namespace &);
		
	virtual void VisitConstructor(const Constructor & ctor)
	{
		MACARONI_ASSERT(false,
			"Cannot generate code for a constructor here!");
	}

	virtual void VisitDestructor(const Destructor & ctor)
	{
		MACARONI_ASSERT(false,
			"Cannot generate code for a denstructor here!");
	}

	virtual void VisitAdoptedFunction(const Macaroni::Model::Cpp::Function & func)
	{
		writeFunction(func);
	}

	virtual void VisitFunction(const Macaroni::Model::Cpp::Function & func)
	{
		if (func.GetNode()->GetAdoptedHome() != nullptr)
		{
			return;
		}
		writeFunction(func);
	}

	virtual void writeFunction(const Macaroni::Model::Cpp::Function & func)
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

		writeFunctionArgumentList(func);

		if (func.IsConst())
		{
			write(" const");
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

	virtual void VisitAdoptedVariable(const Macaroni::Model::Cpp::Variable & var)
	{	
		writeVariable(var);
	}

	virtual void VisitVariable(const Macaroni::Model::Cpp::Variable & var)
	{	
		if (var.GetNode()->GetAdoptedHome() != nullptr)
		{
			return;
		}
		writeVariable(var);
	}

	virtual void writeVariable(const Macaroni::Model::Cpp::Variable & var)
	{	
		writeTypeInfoHFile(var.GetType());

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

	
	void writeFunctionArgumentList(const Macaroni::Model::Cpp::Function & func)
	{
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
				writeTypeInfo(arg->GetType());
				//write(" ");
				write(arg->GetName());			
			}
			depth --;
			startLine();
			write(")");
		}
	}

	void writeHeaderCompileGuard(const Member & m)
	{
		hFile << "#ifndef MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_H\n";
		hFile << "#define MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_H\n";
		cppFile << "#ifndef MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_CPP\n";
		cppFile << "#define MACARONI_COMPILEGUARD_" << m.GetNode()->GetPrettyFullName("_") << "_CPP\n";
		
		startLine();
	}

	inline static bool isPrimitive(const NodePtr node)
	{
		MemberPtr member = node->GetMember();
		return !!member && !!(boost::dynamic_pointer_cast<Primitive>(member));
	}

	void writeHeaderIncludeStatements(const Member & m, NodeListPtr imports)
	{		
		for(unsigned int i = 0; i < imports->size(); i ++)
		{
			NodePtr import = (*imports)[i];
			if (!isPrimitive(import))
			{
				std::ofstream & file = 
					(m.DoesDefinitionReference(import)) ? hFile : cppFile;	
				file << "#include " << import->GetHFilePath() << "\n";
			}
		}
		startLine();
	}

	void writeHeaderUsingStatements(NodeListPtr imports)
	{
		for(unsigned int i = 0; i < imports->size(); i ++)
		{
			NodePtr import = (*imports)[i];					
			if (!isPrimitive(import))
			{
				cppFile << "using " << import->GetPrettyFullName("::") << ";\n";
			}
		}
		startLineCppFile();
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

	

	void writeTypeInfo(const TypePtr type)
	{
		if (type->IsConst())
		{
			write("const ");			
		}
		hFile << type->GetNode()->GetPrettyFullName("::") <<  " ";
		cppFile << type->GetNode()->GetPrettyFullName("::") << " ";
		if (type->IsPointer())
		{
			write("* ");			
			if (type->IsConstPointer())
			{
				write("const ");				
			}
		}
		if (type->IsReference())
		{
			write("& ");			
		}		
	}

	void writeTypeInfoHFile(const TypePtr type)
	{
		if (type->IsConst())
		{
			hFile << "const ";			
		}
		hFile << type->GetNode()->GetPrettyFullName("::") <<  " ";
		if (type->IsPointer())
		{
			hFile << "* ";			
			if (type->IsConstPointer())
			{
				hFile << "const ";				
			}
		}
		if (type->IsReference())
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

class DoNothingVisitor : public ScopeVisitor
{
public:
	DoNothingVisitor(CppSourceGenerator * parent, std::ofstream & hFile, std::ofstream & cppFile, int depth)
		: ScopeVisitor(parent, hFile, cppFile, depth)
	{
	}

	virtual ~DoNothingVisitor(){};
	
	///*virtual MemberVisitor * VisitClass(const Class &)
	//{
	//	;
	//}*/

	virtual void VisitClassFooter()
	{
	}

	virtual void VisitConstructor(const Constructor &)
	{
	}

	virtual void VisitDestructor(const Destructor &)
	{
	}

	virtual void VisitAdoptedFunction(const Function &)
	{
	}

	virtual void VisitFunction(const Function &)
	{
	}

	///*virtual MemberVisitor * VisitNamespace(const Namespace &)
	//{
	//	return nullptr;
	//}*/

	virtual void VisitAdoptedVariable(const Variable &)
	{
	}

	virtual void VisitVariable(const Variable &)
	{
	}
};

bool isNodeWorthAnEntireNsFile(NodePtr node)
{
	if (node->GetMember() == nullptr)
	{
		return false;
	}
	MemberPtr member = node->GetMember();
	if (!member)
	{
		return false;
	}
	if (!(boost::dynamic_pointer_cast<Class>(member)))
	{
		return false;
	}
	if (!(boost::dynamic_pointer_cast<Function>(member)))
	{
		return node->GetAdoptedHome() == node->GetNode();
	}
	return true;
}

bool isNamespaceWorthAnEntireFile(const Namespace & ns)
{
	// Count to see if there are only classes in this namespace (no functions)
	unsigned int i = 0;
	for(i = 0; i < ns.GetNode()->GetChildCount(); i ++)
	{
		NodePtr child = ns.GetNode()->GetChild(i);
		if (isNodeWorthAnEntireNsFile(child))
		{
			return true;
		}
	}
	return false;
}

MemberVisitor * ScopeVisitor::VisitNamespace(const Macaroni::Model::Cpp::Namespace & ns)
{
	///*if (!isNamespaceWorthAnEntireFile(ns))
	//{
	//	return new DoNothingVisitor(parent, 
	//		hFile,
	//		cppFile,
	//		0);;
	//}*/

	std::auto_ptr<std::ofstream> hFilePtr(new std::ofstream);
	std::auto_ptr<std::ofstream> cppFilePtr(new std::ofstream);

	if (ns.GetNode()->GetNode() != nullptr)
	{
		path dirPath(parent->GetRootPath());
		dirPath /= ns.GetNode()->GetPrettyFullName("/");
		boost::filesystem::create_directory(dirPath);
	}

	std::stringstream ss;
	ss << parent->GetRootPath().string() << "/" << ns.GetNode()->GetPrettyFullName("/") << "/global.h";	
	openFileIfPossible(*(hFilePtr.get()), ss.str().c_str());
	//hFilePtr->open(ss.str().c_str(), std::ios::out);
	//MACARONI_ASSERT(hFilePtr->is_open(), "HFile not opened.");

	std::string p = ss.str();

	ss.str("");
	ss << parent->GetRootPath().string() << "/" << ns.GetNode()->GetPrettyFullName("/") << "/global.cpp";
	openFileIfPossible(*(cppFilePtr.get()), ss.str().c_str());
	//cppFilePtr->open(ss.str().c_str(), std::ios::out);
	//MACARONI_ASSERT(cppFilePtr->is_open(), "CppFile not opened.");
	
	MemberVisitor * mv = new NamespaceVisitor(parent, 
			dynamic_cast<std::ofstream &>(*hFilePtr.get()),
			dynamic_cast<std::ofstream &>(*cppFilePtr.get()),
			0,
			ns);
	
	hFilePtr.release();
	cppFilePtr.release();
	return mv;
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
		cppFile << "#include \"" << fileClass.GetNode()->GetName() << ".h\"\n";
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
		writeFooter(fileClass);

		delete &hFile;
		delete &cppFile;
	}

	void VisitClassFooter()
	{
		hFile << "\n";
		hFile << "}; // End of class " << fileClass.GetName() << "\n";
	}

	virtual void VisitConstructor(const Constructor & ctor)
	{
		write(fileClass.GetNode()->GetName());
		writeFunctionArgumentList(ctor);
		startLineHFile();
		startLineCppFile();		
		
		depth ++;
		for(int i = 0; i < ctor.GetAssignmentCount(); i ++)
		{
			startLineCppFile();
			if (i == 0)
			{
				cppFile << ":\n";
			}
			const VariableAssignment & init = ctor.GetAssignment(i);
			cppFile << init.Variable->GetName() 
				    << "(" << init.Expression << ")";
			if (i < ctor.GetAssignmentCount() - 1)
			{
				cppFile << ", ";
			}
			startLineCppFile();
		}
		depth --;
		startLineCppFile();
		cppFile << "{";
		depth ++;
		//startLineCppFile();
		cppFile << ctor.GetCodeBlock();
		depth --;
		startLineCppFile();
		cppFile << "}";
		startLineCppFile();
	}

	virtual void VisitDestructor(const Destructor & dtor)
	{
		write("~");
		write(fileClass.GetNode()->GetName());
		writeFunctionArgumentList(dtor);
		startLineHFile();
		startLineCppFile();
		cppFile << "{";
		depth ++;
		//startLineCppFile();
		cppFile << dtor.GetCodeBlock();
		depth --;
		startLineCppFile();
		cppFile << "}";
		startLineCppFile();
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
	openFileIfPossible(*(hFilePtr.get()), ss.str().c_str());
	//hFilePtr->open(ss.str().c_str(), std::ios::out);
	//MACARONI_ASSERT(hFilePtr->is_open(), "HFile not opened.");

	std::string p = ss.str();

	ss.str("");
	ss << parent->GetRootPath().string() << "/" << c.GetNode()->GetPrettyFullName("/") << ".cpp";
	openFileIfPossible(*(cppFilePtr.get()), ss.str().c_str());
	//cppFilePtr->open(ss.str().c_str(), std::ios::out);
	//MACARONI_ASSERT(cppFilePtr->is_open(), "CppFile not opened.");
	
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
	openFileIfPossible(hFile, ss.str().c_str());
	/*if (isOkToOverwriteFile(ss.str().c_str()))
	{
		hFile.open(ss.str().c_str(), std::ios::out);
		MACARONI_ASSERT(hFile.is_open(), "HFile not opened.");
		hFile << std::string(MACARONI_OVERWRITE_ALLOWED) << "\n";
	}*/

	std::string s = ss.str();

	ss.str("");
	ss << rootPath.string() << "\\global.cpp";	
	openFileIfPossible(cppFile, ss.str().c_str());
	/*if (isOkToOverwriteFile(ss.str().c_str()))
	{
		cppFile.open(ss.str().c_str(), std::ios::out);
		MACARONI_ASSERT(cppFile.is_open(), "CppFile not opened.");
		cppFile << std::string(MACARONI_OVERWRITE_ALLOWED) << "\n";
	}*/

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

