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
#ifndef MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP
#define MACARONI_PARSER_PIPPY_PIPPYPARSER_CPP

#include <algorithm>
#include <cctype>
#include <Macaroni/ME.h>
#include <Macaroni/Model/AnnotationTable.h>
#include <Macaroni/Model/AnnotationValue.h>
#include <Macaroni/Model/AnnotationValuePtr.h>
#include <Macaroni/Model/Cpp/Access.h>
#include <Macaroni/Model/Cpp/AccessPtr.h>
#include <Macaroni/Model/Block.h>
#include <Macaroni/Model/Cpp/Class.h>
#include <Macaroni/Model/Cpp/Constructor.h>
#include <Macaroni/Model/Cpp/ConstructorOverload.h>
#include <Macaroni/Model/Cpp/ConstructorPtr.h>
#include <Macaroni/Model/Context.h>
#include <Macaroni/Parser/Cpp/CppAxioms.h>
#include <Macaroni/Model/Cpp/CppContext.h>
#include <Macaroni/Model/CvQualifier.h>
#include <Macaroni/Model/Cpp/Destructor.h>
#include <Macaroni/Model/Cpp/DestructorPtr.h>
#include <Macaroni/Model/Cpp/Enum.h>
#include <Macaroni/Model/Cpp/EnumPtr.h>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/Cpp/ExceptionSpecifier.h>
#include <Macaroni/Model/Project/ExeTarget.h>
#include <Macaroni/Model/Project/ExeTargetPtr.h>
#include <Macaroni/Model/Cpp/Extern.h>
#include <Macaroni/Model/FileName.h>
#include <boost/foreach.hpp>
#include <Macaroni/Model/Cpp/Function.h>
#include <Macaroni/Model/Cpp/FunctionOverload.h>
#include <Macaroni/Model/Import.h>
#include <Macaroni/Model/ImportList.h>
#include <memory>
#include <Macaroni/Environment/Messages.h>
#include <Macaroni/Model/ModelInconsistencyException.h>
#include <Macaroni/Model/Cpp/Namespace.h>
#include <Macaroni/Model/Node.h>
#include "PippyParser.h"
#include <Macaroni/Parser/Parser.h>
#include <Macaroni/Parser/ParserException.h>
#include <Macaroni/IO/Path.h>
#include <Macaroni/Model/Cpp/Primitive.h>
#include <Macaroni/Model/Reason.h>
#include <Macaroni/Model/SimpleTypeModifiers.h>
#include <Macaroni/Model/Source.h>
#include <Macaroni/StringException.h>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Model/Project/TargetPtr.h>
#include <Macaroni/Model/Cpp/TemplateTypename.h>
#include <Macaroni/Model/Cpp/TemplateParameterList.h>
#include <boost/algorithm/string/trim.hpp>
#include <Macaroni/Model/Type.h>
#include <Macaroni/Model/TypeArgumentListPtr.h>
#include <Macaroni/Model/TypeModifiers.h>
#include <Macaroni/Model/Cpp/Typedef.h>
#include <Macaroni/Model/Cpp/TypeInfo.h>
#include <Macaroni/Model/Project/UnitTarget.h>
#include <Macaroni/Model/Cpp/Variable.h>
#include <Macaroni/Model/Cpp/VariableAssignment.h>

//#include "PippyParser.spirit"
#include <sstream>
#include <string>

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::AccessPtr;
using Macaroni::Model::AnnotationTable;
using Macaroni::Model::AnnotationTableInternalPtr;
using Macaroni::Model::AnnotationTablePtr;
using Macaroni::Model::AnnotationValue;
using Macaroni::Model::AnnotationValueInternalPtr;
using Macaroni::Model::AnnotationValuePtr;
using std::auto_ptr;
using Macaroni::Model::Block;
using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr;
using Macaroni::Model::Cpp::Constructor;
using Macaroni::Model::Cpp::ConstructorOverload;
using Macaroni::Model::Cpp::ConstructorOverloadPtr;
using Macaroni::Model::Cpp::ConstructorPtr;
using Macaroni::Model::Context;
using Macaroni::Model::ContextPtr;
using namespace Macaroni::Parser::Cpp;
using Macaroni::Model::Cpp::CppContext;
using Macaroni::Model::CvQualifier;
using Macaroni::Model::Cpp::Destructor;
using Macaroni::Model::Cpp::DestructorPtr;
using Macaroni::Model::Cpp::Enum;
using Macaroni::Model::Cpp::EnumPtr;
using Macaroni::Model::Cpp::ExceptionSpecifier;
using Macaroni::Model::Project::ExeTarget;
using Macaroni::Model::Project::ExeTargetPtr;
using Macaroni::Model::Cpp::Extern;
using Macaroni::Model::FileName;
using Macaroni::Model::FileNamePtr;
using Macaroni::Model::Cpp::Function;
using Macaroni::Model::Cpp::FunctionOverload;
using Macaroni::Model::Cpp::FunctionOverloadPtr;
using Macaroni::Model::Cpp::FunctionPtr;
using Macaroni::Model::Import;
using Macaroni::Model::ImportList;
using Macaroni::Model::ImportType;
using Macaroni::Model::Project::LibraryTarget;
using Macaroni::Model::Project::LibraryTargetPtr;
using Macaroni::Environment::Messages;
using Macaroni::Model::ModelInconsistencyException;
using Macaroni::Model::Cpp::Namespace;
using Macaroni::Model::Node;
using Macaroni::Model::NodeList;
using Macaroni::Model::NodeListPtr;
using Macaroni::Model::NodePtr;
using Macaroni::Parser::ParserException;
using Macaroni::IO::PathListPtr;
using Macaroni::Model::Cpp::Primitive;
using Macaroni::Model::Reason;
using Macaroni::Model::ReasonPtr;
using Macaroni::Model::SimpleTypeModifiers;
using Macaroni::Model::Source;
using Macaroni::Model::SourcePtr;
using Macaroni::StringException;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;
using Macaroni::Model::Cpp::TemplateTypename;
using Macaroni::Model::Cpp::TemplateParameterList;
using Macaroni::Model::Type;
using Macaroni::Model::TypeArgumentListPtr;
using Macaroni::Model::Cpp::Typedef;
using Macaroni::Model::Cpp::TypedefPtr;
using Macaroni::Model::Cpp::TypeInfo;
using Macaroni::Model::TypeModifiers;
using Macaroni::Model::TypePtr;
using Macaroni::Model::Project::UnitTarget;
using Macaroni::Model::Cpp::Variable;
using Macaroni::Model::Cpp::VariableAssignment;

BEGIN_NAMESPACE(Macaroni, Parser, Pippy)

typedef std::string::const_iterator StringItr;

void Assert(bool b)
{
	if (!b)
	{
		MACARONI_FAIL("Fail!");
	}
}

class Iterator
{
public:
	Iterator(StringItr itr, StringItr end, SourcePtr source)
		: column(source->GetColumn()),  endItr(end),
		fileName(source->GetFileName()),
		itr(itr), line(source->GetLine())
	{

	}

	inline void Advance(int count)
	{
		for(int i = 0; i < count; i ++)
		{
			if (itr == endItr)
			{
				MACARONI_ASSERT(false, "Advanced past end of string.");
			}
			if (*itr == '\n')
			{
				line ++;
				column = 0;
			}
			itr ++;
			column ++;
		}
	}

	// Returns current line.
	inline char Current()
	{
		MACARONI_ASSERT(!Finished(), "Cannot get Current() if finished.");
		return *itr;
	}

	inline bool ConsumeChar(const char c)
	{
		if (Is(c))
		{
			Advance(1);
			return true;
		}
		return false;
	}

	inline bool ConsumeWord(const char * word)
	{
		if (Is(word))
		{
			Advance(strlen(word));
			return true;
		}
		return false;
	}

	SourcePtr GetSource()
	{
		return Source::Create(fileName, line, column);
	}

	SourcePtr GetSource(int plusLines, int plusColumns)
	{
		return Source::Create(fileName, line + plusLines, column + plusColumns);
	}

	// Consumes whitespace. Returns amount consumed.
	inline int ConsumeWhitespace()
	{
		int consumed = 0;
		while(!Finished() && IsWhiteSpace())
		{
			consumed ++;
			Advance(1);
		}
		return consumed;
	}

	inline bool Finished()
	{
		return (itr == endItr);
	}

	inline bool Is(const char other)
	{
		return Finished() ? false : (*itr) == other;
	}

	inline bool Is(const char * string)
	{
		StringItr src = itr;

		while(src != endItr)
		{
			if (*string == '\0')
			{
				return true;
			}
			if (*src != *string)
			{
				return false;
			}
			src ++;
			string ++;
		}

		return *string == '\0';
	}

	inline bool IsWhiteSpace()
	{
		return *itr <= 32;
	}

	static void IsTests()
	{
		std::string ok("12ComplexWord");
		FileNamePtr file = FileName::CreateNonPhysical("Blah.mcpp");
		SourcePtr src = Source::Create(file, 1, 1);
		Iterator itr(ok.begin(), ok.end(), src);

		Assert(itr.Is("12ComplexWord"));
		Assert(!itr.Is("2Complex"));
		Assert(itr.Is("12"));
		Assert(itr.Is("12Com"));
		Assert(itr.Is('1'));
		Assert(!itr.Is('2'));
		itr.Advance(1);
		Assert(itr.Is('2'));
		itr.Advance(1);
		Assert(itr.Is("Complex"));
		Assert(itr.Is("ComplexWord"));
		itr.Advance(7);
		Assert(itr.Is("Word"));
		Assert(!itr.Is("C"));
		try
		{
			itr.Advance(5);
			// Advances one past Word, going out of bounds.
			// Thus, an exception must be thrown.
			Assert(false);
		}
		catch(Macaroni::Exception &)
		{
			// Good
		}
	}

	inline bool IsAlpha()
	{
		return Finished() ? false : IsAlpha(*itr);
	}

	static inline bool IsAlpha(const char c)
	{
		return (65 <= c && c <= 90) ||
				(97 <= c && c <= 122);
	}

	static void IsAlphaTests()
	{
		Assert(IsAlpha('a'));
		Assert(IsAlpha('z'));
		Assert(IsAlpha('m'));
		Assert(IsAlpha('A'));
		Assert(IsAlpha('Z'));
		Assert(IsAlpha('M'));
		Assert(!IsAlpha('1'));
		Assert(!IsAlpha('['));
		Assert(!IsAlpha('@'));
		Assert(!IsAlpha('\''));
		Assert(!IsAlpha('{'));
	}

	inline bool IsDigit()
	{
		return Finished() ? false : IsDigit(*itr);
	}

	static inline bool IsDigit(const char c)
	{
		return c >=48 && c <= 57;
	}

	static void IsDigitTests()
	{
		Assert(IsDigit('0'));
		Assert(IsDigit('1'));
		Assert(IsDigit('2'));
		Assert(IsDigit('3'));
		Assert(IsDigit('4'));
		Assert(IsDigit('5'));
		Assert(IsDigit('6'));
		Assert(IsDigit('7'));
		Assert(IsDigit('8'));
		Assert(IsDigit('9'));
		Assert(!IsDigit('a'));
		Assert(!IsDigit('!'));
		Assert(!IsDigit(':'));
		Assert(!IsDigit('/'));
	}

	bool IsAtSamePositionAs(const Iterator & other)
	{
		return this->column == other.column
			&& this->line == other.line;
	}

	static void RunTests()
	{
		IsAlphaTests();
		IsDigitTests();
		IsTests();
	}
private:
	int column;
	StringItr endItr;
	FileNamePtr fileName;
	StringItr itr;
	int line;
};

static std::string unsafeStaticNightmareString;

static Iterator createTestItr(const char * msgChars)
{
	if (msgChars == nullptr)
	{
		MACARONI_THROW("Cannot pass nullptr to createTestItr.")
	}
	unsafeStaticNightmareString = std::string(msgChars);
	FileNamePtr file = FileName::CreateNonPhysical("Blah.mcpp");
	SourcePtr src = Source::Create(file, 1, 1);
	Iterator p(unsafeStaticNightmareString.begin(),
			   unsafeStaticNightmareString.end(),
			   src);
	return p;
}



class ParserFunctions
{
private:
	int classDepth;
	ContextPtr context;
	AccessPtr currentAccess;
	NodeList currentTemplateParameters;
	NodePtr currentScope;
	TargetPtr currentTarget;
	TargetPtr defaultTarget;
	std::vector<ImportList *> importedNodesStack;
	AccessPtr lastAccess;
	ImportList topScopeImportedNodes;
public:

	ParserFunctions(ContextPtr context, TargetPtr target)
		:classDepth(0),
		 context(context),
		 currentAccess(Access::NotSpecified()),
		 currentTemplateParameters(),
		 currentScope(context->GetRoot()),
		 currentTarget(target),
		 defaultTarget(target),
		 importedNodesStack(),
		 lastAccess(Access::NotSpecified()),
		 topScopeImportedNodes()
	{
		MACARONI_ASSERT(!!CppContext::GetPrimitives(context),
			"Cpp nodes must be found to parse successfully.");
		NodePtr primitiveRoot = CppContext::GetPrimitives(context);
		for(unsigned int i = 0; i < primitiveRoot->GetChildCount(); i ++)
		{
			Node & n = *(primitiveRoot->GetChild(i));
			topScopeImportedNodes.emplace_back(n, ImportType::Normal);
		}
		importedNodesStack.push_back(&topScopeImportedNodes);
	}

	/** Attempts to consume whatever access it can, returning
	 * "Access_NotSpecified" if it finds nothing. */
	AccessPtr AccessKeyword(Iterator & itr)
	{
		using Macaroni::Model::Cpp::Access;

		ConsumeWhitespace(itr);
		AccessPtr access = Access::NotSpecified();
		if (itr.ConsumeWord("~hidden"))
		{
			itr.ConsumeWhitespace();
			if (itr.ConsumeWord("public"))
			{
				access = Access::HiddenPublic();
			}
			else
			{
				access = Access::Hidden();
			}
		}
		// TODO: The "~internal" keyword idea is going to be a lot harder than
		// I first thought.  In MSVC++ a class's members must all be exported
		// if it is exported. So this would make sense for classes... later.
		/*else if (itr.ConsumeWord("~internal"))
		{
			itr.ConsumeWhitespace();
			if (itr.ConsumeWord("protected"))
			{
				access = Access::InternalProtected();
			}
			else
			{
				access = Access::Internal();
			}
		}*/
		else if (itr.ConsumeWord("private"))
		{
			access = Access::Private();
		}
		else if (itr.ConsumeWord("protected"))
		{
			/*itr.ConsumeWhitespace();
			if (itr.ConsumeWord("~internal"))
			{
				access = Access::InternalProtected();
			}
			else
			{
				access = Access::Internal();
			}*/
			access = Access::Protected();
		}
		else if (itr.ConsumeWord("public"))
		{
			access = Access::Public();
		}
		else
		{
			access = Access::NotSpecified();
		}

		if (*access != *Access::NotSpecified()) {
			itr.ConsumeWhitespace();
			if (itr.ConsumeChar(':')) {
				currentAccess = access;
			}
		}
		return access;
	}

	void AddImport(const ImportType type, NodePtr node)
	{
		AddImport(type, node.get());
	}

	void AddImport(const ImportType type, Node * node)
	{
		MACARONI_ASSERT(node != nullptr, "null node for import");
		currentImports().emplace_back(*node, type);
	}

	bool Annotation(Iterator & itr)
	{
		if (!itr.ConsumeChar('@'))
		{
			return false;
		}
		// COMMITTED
		if (!this->currentScope)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Annotation.AnnotationFoundOutsideOfAnyScope"));
		}
		NodePtr name;
		if (!ConsumeNodeName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Annotation.NodeNameExpectedAfterAt"));
		}

		AnnotationValueInternalPtr annotationValue;
		Iterator attrBegin = itr;
		if (AnnotationValue_(itr, attrBegin, name, annotationValue))
		{
			itr.ConsumeWhitespace();
			// Changing this, because if its in a typedef or Variable its
			// ambiguous.
			// itr.ConsumeChar(';'); // Consume this (its optional)
			currentScope->GetAnnotations().Add(annotationValue);
			return true;
		}
		throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Annotation.ValueExpectedFollowingNodeName"));
	}

	bool AnnotationValue_(Iterator & itr, Iterator & attrBegin,
						 const NodePtr & name,
						 AnnotationValueInternalPtr & annotationValue)
	{
		itr.ConsumeWhitespace();
		// At this point, can see the following:
		// bool - true or false,
		// node - another node name
		// number - a number literal
		// string - a string in the form "blah", {blah}, or (blah)
		// table - starts with [

		bool valueBool;
		double valueNumber;
		NodePtr valueNode;
		std::string valueString;
		if (AnnotationValueBool(itr, valueBool))
		{
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name, valueBool,
					Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AnnotationValueNumber(itr, valueNumber))
		{
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name, valueNumber,
					Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AnnotationValueNode(itr, valueNode))
		{
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name, valueNode,
					Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
		}
		else if (AnnotationValueString(itr, valueString))
		{
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name, valueString,
					Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
		}
		else if (itr.ConsumeChar('['))
		{
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name,
					Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
			annotationTableContents(itr, annotationValue->GetValueAsTable());
		}
		else
		{
			// Short hand syntax allows for an empty annotation.
			// For now this is a table but it'd be cool if it could be nothing
			// to avoid the waste.
			annotationValue = AnnotationValueInternalPtr(new AnnotationValue(name,
				Reason::Create(CppAxioms::AnnotationValueCreation(), attrBegin.GetSource())
				));
		}
		return !!annotationValue;
	}


	void annotationTableContents(Iterator & itr, const AnnotationTablePtr & table)
	{
		itr.ConsumeWhitespace();
		while(!itr.Is(']'))
		{
			Iterator attrBegin = itr;
			std::string name;
			if (!ConsumeSimpleName(itr, name))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Annotation.EndBracketOrSimpleNameExpectedInsideAnnotationTable"));
			}
			itr.ConsumeWhitespace();
			if (!itr.ConsumeChar('='))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Annotation.TableEntryEqualsExpected"));
			}
			itr.ConsumeWhitespace();

			NodePtr home = table->GetHomeNode();
			NodePtr entryName = home->FindOrCreate(name);

			AnnotationValueInternalPtr annotationValue;
			if (!AnnotationValue_(itr, attrBegin, entryName, annotationValue))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Annotation.TableEntryValueExpected"));
			}
			table->Add(annotationValue);
			itr.ConsumeWhitespace();
			if (itr.ConsumeChar(',')) // optional
			{
				itr.ConsumeWhitespace();
			}
		}
		itr.Advance(1);
	}

	bool AnnotationValueBool(Iterator & itr, bool & value)
	{
		if (itr.ConsumeWord("true"))
		{
			value = true;
			return true;
		}
		if (itr.ConsumeWord("false"))
		{
			value = false;
			return true;
		}
		return false;
	}

	bool AnnotationValueNode(Iterator & itr, NodePtr & value)
	{
		if (this->ConsumeNodeName(itr, value))
		{
			if (!value)
			{
				// Means it looked like a Node, but no Node with that
				// name was found.
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Annotation.ValueNoNodeByTheGivenNameFound"));
			}
			return true;
		}
		return false;
	}

	bool AnnotationValueNumber(Iterator & itr, double & value)
	{
		return RealNumber(itr, value);
	}

	bool AnnotationValueString(Iterator & itr, std::string & value)
	{
		if (ConsumeStringLiteral(itr, value))
		{
			return true;
		}
		if (itr.ConsumeChar('='))
		{
			SourcePtr startOfBlock;
			if (CodeBlock(itr, value, startOfBlock))
			{
				return true;
			}
			else
			{
				throw ParserException(itr.GetSource(), Messages::Get(
					"CppParser.Annotation.CodeBlockExpectedAfterEquals"));
			}
		}
		return false;
	}

	bool Block(Iterator & itr)
	{
		{
			Iterator newItr = itr;
			ConsumeWhitespace(newItr);
			if (!newItr.ConsumeWord("~block"))
			{
				return false;
			}

			// We're committed.
			itr = newItr;
		}

		itr.ConsumeWhitespace();
		std::string id;

		if (!ConsumeStringLiteral(itr, id))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.StringLiteralStartExpected"));
		}

		// New Macaroni IV error checking.
		if (id == "top" || id == "bottom")
		{
			if (currentScope->HasElementOfType<Macaroni::Model::Cpp::Class>())
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockTopBottomMustBeOnUnit"));
			}
			if (dynamic_cast<LibraryTarget *>(currentTarget.get()) != nullptr)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockTopBottomMustHaveUnit"));
			}
		}
		else if (id == "h" || id == "cpp")
		{
			// Force a unit for these.
			if (dynamic_cast<LibraryTarget *>(currentTarget.get()) != nullptr
				&&
				!(currentScope->HasElementOfType<Macaroni::Model::Cpp::Class>())
				)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockMustHaveUnit"));
			}
		}
		else if (id == "h-predef")
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockHPredefNotAllowed"));
		}
		else if (id == "h-postdef")
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockHPostdefNotAllowed"));
		}
		else if (id != "cpp-include")
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.BlockIdUnknown"));
		}


		itr.ConsumeWhitespace();
		std::string code;
		SourcePtr codeStart;
		bool readToEof = itr.ConsumeWord(":=");
		if (!CodeBlock(itr, code, codeStart, readToEof))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.CodeBlock.Expected"));
		}
		NodePtr blockHome;
		blockHome = createNextBlockNode(currentScope, itr);

		TargetPtr tHome;
		boost::optional<ImportList> imports = boost::none;
		//TODO:
		// Change to:
		//if (!currentScope->HasElementOfType<Macaroni::Model::Cpp::Class>())
		if (!blockHome->GetNode() ||
			!blockHome->GetNode()->HasElementOfType<Macaroni::Model::Cpp::Class>())
		{
			imports = currentImports();
			tHome = deduceTargetHome(currentScope);
		}
		// NOTE! tHome *can* be null here!
		Block::Create(tHome, blockHome, id, code,
			Reason::Create(CppAxioms::BlockCreation(), codeStart),
			imports);

		// For the newer generators, we need to force an import on everything
		// that comes afterwards for the blocks to generate in the proper
		// order.
		// UPDATE: Unless we're in a class, as we'll give the class containing
		// the import and dependency on it and barf the whole thing up!
		if (!weAreInsideAClass()) {
			if (id == "h" || id == "h-predef") {
				AddImport(ImportType::H, blockHome);
			} else if (id == "cpp" || id == "cpp-include") {
				AddImport(ImportType::Normal, blockHome);
			}
		}
		return true;
	}

	// looks for "class [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Class(Iterator & itr, NodePtr templateHome=NodePtr{})
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);

		AccessPtr access = AccessKeyword(newItr);
		newItr.ConsumeWhitespace();

		auto defaultAccess = Access::Private();
		bool isStruct = false;

		if (!newItr.ConsumeWord("class"))
		{
			if (!newItr.ConsumeWord("struct"))
			{
				return false;
			}
			else
			{
				isStruct = true;
				defaultAccess = Access::Public();
			}
		}

		// Increment depth, and also decrement it.
		struct DepthTracker {

			int & depth;

			DepthTracker(int & depth)
			:	depth(depth)
			{
				++ depth;
			}

			~DepthTracker()
			{
				-- depth;
			}
		} classDepthTracker(this->classDepth);

		// Once the parser has seen "class" it must see the correct
		// grammar or there will be hell to pay.

		ConsumeWhitespace(newItr);

		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoID1"));
		}

		NodePtr oldScope = currentScope;
		::Macaroni::Model::Cpp::Class * enclosingClass
			= oldScope->GetElementPtr<::Macaroni::Model::Cpp::Class>();
		currentScope = currentScope->FindOrCreate(name);
		lastAccess = currentAccess;
		currentAccess = defaultAccess;

		ClassPtr newClass;

		// We may have to create a new UnitTarget for this, so save the old
		// target.
		TargetPtr formerTarget = currentTarget;

		TargetPtr tHome = deduceTargetHome(currentScope);
		newClass = Class::Create(
			tHome, currentScope, isStruct, access, currentImports(),
			Reason::Create(CppAxioms::ClassCreation(), newItr.GetSource()),
			templateHome, enclosingClass);

		// Switch to using the class's import list for any statements we see
		// next. We'll pop it when we're done.
		importedNodesStack.push_back(&(newClass->GetMutableImportedNodes()));

		if (templateHome)
		{
			// Move the template to their real home...
			context->LandFloater(currentScope);
		}

		ClassParents(newItr, newClass);

		ConsumeWhitespace(newItr);

		while(Annotation(newItr));

		ConsumeWhitespace(newItr);

		SourcePtr firstBraceSrc = newItr.GetSource();

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoOpeningBrace"));
			//EXCEPTION MSG: Expectedd { after namespace identifier.")
		}

		// Calls the parser to keep on parsing until the class ends.


		// It makes sense to create a unit just for a class the moment we see
		// it, but the use of the ~hfile keyword (which was like a crappier yet
		// more complex way of doing what ~extern does) makes this a bit harder
		// than it should be.

		const bool hasHFile = HFileDirective(newItr);
		if (hasHFile)
		{
			// leave this the hell alone.
		}
		else if (currentTargetIsLibrary())
		{
			// We're just chilling *wherever* and have defined a class.
			// Assume we want to make an implicit UnitTarget.

			// Macaroni allows partial class definitions (WHY DID I DO THIS GOD?!)
			// so it's possible there was already a target created for this class
			// earlier.
			if (newClass->GetOwner() != formerTarget)
			{
				// Make an assumption this target was the implicit target.
				if (!newClass->GetOwner()->IsImplicit())
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Class.ExplicitToImplictError"));
				}

				// Just use the old target.
				currentTarget = newClass->GetOwner();
			}
			else
			{
				// Make a brand new target.
				currentTarget = createImplicitUnitForNode(*currentScope);
				newClass->SwitchOwner(currentTarget);
			}
		}
		else
		{
			// Class previously was defined in a unit.

			// So if we aren't just wherever, but are in an explicit target,
			// then the old class whose def we picked better match this one.
			if (newClass->GetOwner() != formerTarget)
			{
				throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Class.ImplicitToExplicitError"));
			}
		}

		ScopeFiller(newItr);

		ConsumeWhitespace(newItr);

		//Model::Cpp::Namespace::Create(currentScope,
		//
	///		);

		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeChar('}'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Class.NoEndingBrace", firstBraceSrc->GetLine()));
		}

		// The ';' following a class def is optional.
		ConsumeWhitespace(newItr);
		newItr.ConsumeChar(';');

		currentTarget = formerTarget; // Switch back to the other target.

		itr = newItr; // Success! :)
		currentScope = oldScope;
		currentAccess = lastAccess;
		importedNodesStack.pop_back();

		return true;
	}

	bool ClassParents(Iterator & itr, ClassPtr & newClass)
	{
		using namespace Macaroni::Model::Cpp;

		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar(':')) {
			return false;
		}
		do
		{
			itr.ConsumeWhitespace();
			AccessPtr access = AccessKeyword(itr);
			if ((*access) == (*(Access::NotSpecified())))
			{
				access = Access::Private();
			}

			bool _virtual = VirtualKeyword(itr);

			itr.ConsumeWhitespace();

			TypePtr type;
			if (!Type_(itr, type))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Class.Inheritance.NoType"));
			}
			newClass->AddParent(type, access, _virtual);
			itr.ConsumeWhitespace();
		} while (itr.ConsumeChar(','));
		return true;
	}

	/** Consumes white space, then returns false if '{' not seen.
	 * If it is, consumes all text until next '}'.
	 * If readToEof is set, reads till the end of file instead. */
	bool CodeBlock(Iterator & itr, std::string & codeBlock, SourcePtr & start,
		           bool readToEof=false)
	{
		ConsumeWhitespace(itr);
		std::stringstream ss;
		if (readToEof)
		{
			start = itr.GetSource();
			while (!itr.Finished())
			{
				ss << itr.Current();
				itr.Advance(1);
			}
		}
		else
		{
			if (!itr.Is('{'))
			{
				return false;
			}
			itr.Advance(1);
			start = itr.GetSource();

			int depth = 1;
			while(depth > 0)
			{
				if (itr.Is('{'))
				{
					depth ++;
				}
				if (itr.Is('}'))
				{
					depth --;
					if (depth <= 0)
					{
						itr.Advance(1);
						break;
					}
				}
				char next = itr.Current();
				if (next == '\n')
				{
					ss << "\n";
				}
				else if (next == 9)
				{
					ss << "    ";
				}
				else if (next <= 32)
				{
					ss << ' ';
				}
				else
				{
					ss << next;
				}
				//ss << itr.Current();
				itr.Advance(1);
			}
		}
		// Add code block
		codeBlock = ss.str();
		return true;
	}

	static int ComplexName(Iterator itr)
	{
		int ignore;
		return ComplexName(itr, ignore);
	}

	/** Returns true if a complex name can be parsed from this location.
	 *  "endWhiteSpaceOfKeyword" refers to the index whitespace before some
	 *  keyword ends, and only if Macaroni needs to store a single space before
	 *  that keyword in the complexName.  For example, if the "signed" or
	 *  "unsigned" keyword is found, endIndexOfSignedKeyword
	 *  returns the index where the keyword and its trailing whitespace ends. */
	static int ComplexName(Iterator itr, int & endWhiteSpaceOfKeyword)
	{
		int consumed = 0;
		int simpleNameLength = 0;
		endWhiteSpaceOfKeyword = -1;

		if ((simpleNameLength = SignedOrUnsignedKeyword(itr)) > 0)
		{
			consumed += simpleNameLength;
			itr.Advance(simpleNameLength);
			consumed += itr.ConsumeWhitespace();
			endWhiteSpaceOfKeyword = consumed - 1;
		}
		while(true)
		{
			if (itr.Is("::"))
			{
				itr.Advance(2);
				consumed += 2;
			}
			else if ((simpleNameLength =
			    OperatorName(itr, endWhiteSpaceOfKeyword)) > 0)
			{
				if (endWhiteSpaceOfKeyword > 0)
				{
					// endWhiteSpaceOfKeyword is relative to the operator name,
					// so increase it by whatever we've already consumed.
					endWhiteSpaceOfKeyword += consumed;
				}
				consumed += simpleNameLength;
				itr.Advance(simpleNameLength);
				// We've seen an operator.  A name like
				// Macaroni::operator +::Parser is not valid, so once we've
				// seen one of these exit.
				return consumed;
			}
			else if ((simpleNameLength = SimpleName(itr)) > 0)
			{
				consumed += simpleNameLength;
				itr.Advance(simpleNameLength);
			}
			else
			{
				return consumed;
			}
		}
	}

	static void ComplexNameTests()
	{
		Assert(ComplexName(createTestItr("{dsf")) == 0);
		Assert(ComplexName(createTestItr("abc")) == 3);
		Assert(ComplexName(createTestItr("abc::b3")) == 7);
		Assert(ComplexName(createTestItr("ab_::b3")) == 7);
		Assert(ComplexName(createTestItr("a123::c3")) == 8);
		Assert(ComplexName(createTestItr("::cat::dog")) == 10);
		Assert(ComplexName(createTestItr("::cat::do{")) == 9);
		Assert(ComplexName(createTestItr("")) == 0);
		Assert(ComplexName(createTestItr("13")) == 0);
		Assert(ComplexName(createTestItr("::cat::13")) == 7);
	}


	bool ConstructorOrDestructor(Iterator & itr, NodePtr templateHome=NodePtr{})
	{
		using namespace Macaroni::Model::Cpp;

		Iterator newItr = itr;

		AccessPtr access = Access::NotSpecified();
		bool isInline = false;
		bool isVirtual = false;
		bool isConstExpr = false;
		while(
			(*access == *Access::NotSpecified()
			&& *(access = AccessKeyword(newItr)) != *Access::NotSpecified())
			|| (!isInline && (isInline = InlineKeyword(newItr)))
			|| (!isVirtual && (isVirtual = VirtualKeyword(newItr)))
			|| (!isConstExpr && (isConstExpr = ConstExprKeyword(newItr)))
			)
		{
			//       ~Dog
			// ^._.^
			//  _[[_)(_
		}

		if (*access == *Access::NotSpecified()) {
			access = currentAccess;
		}

		ConsumeWhitespace(newItr);

		bool isExplicit = newItr.ConsumeWord("explicit");

		ConsumeWhitespace(newItr);

		bool tilda = false;
		if (newItr.ConsumeChar('~'))
		{
			tilda = true;
			ConsumeWhitespace(newItr);
		}
		if (!newItr.ConsumeWord(currentScope->GetName().c_str()))
		{
			if (!tilda)
			{
				return false;
			}
			return false;
			/*if (newItr.ConsumeWord("global"))
			{
				// Oh, it wasn't trying to define a destructor, it was just the
				// friendly global keyword! How silly of us!
				return false;
			}
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Constructor.ClassNameExpected")); */
		}
		ConsumeWhitespace(newItr);

		if (!newItr.ConsumeChar('('))
		{
			if (tilda)
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser::Constructor::ArgumentListExpected"));
			}
			else
			{
				return false;
			}
		}

		std::string nodeName(!tilda ? "$ctor" : "$dtor");
		NodePtr ctorNode = currentScope->FindOrCreate(nodeName);

		// Create CTOR or DTOR

		NodePtr fOlNode;
		if (!tilda)
		{
			fOlNode = ConstructorOverload::CreateNode(ctorNode);
			if (templateHome)
			{
				// Move the template to their real home...
				context->LandFloater(fOlNode);
			}
		} // end !tilda
		else
		{
			fOlNode = ctorNode;

		}
		// End create

		NodePtr oldScope = currentScope;
		currentScope = fOlNode; // fOlPtr->GetNode(); //ctorNode;

			FunctionArgumentList(newItr);

		currentScope = oldScope;

		auto overrideKeyword = OverrideKeyword(newItr);

		boost::optional<ExceptionSpecifier> exceptionSpecifier
			= ParseExceptionSpecifier(newItr);

		FunctionOverloadPtr fOlPtr;
		if (!tilda)
		{
			ReasonPtr ctorReason = Reason::Create(CppAxioms::CtorCreation(),
				itr.GetSource());
			ConstructorPtr ctor = Constructor::Create(ctorNode, ctorReason);
			//isInline, access,
			ConstructorOverloadPtr ctorOl =
				ConstructorOverload::Create(fOlNode, isInline, access,
				                            isExplicit, isConstExpr,
				                            exceptionSpecifier,
				                            ctorReason, templateHome);
			fOlPtr = boost::dynamic_pointer_cast<FunctionOverload>(ctorOl);
		} // end !tilda
		else
		{
			if (templateHome)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppAxioms.DestructorCreation.TemplateNotAllowed"));
			}
			DestructorPtr dtor = Destructor::Create(ctorNode,  isInline, access,
				isVirtual, overrideKeyword, exceptionSpecifier,
				Reason::Create(CppAxioms::DtorCreation(), itr.GetSource()));
			fOlPtr = dtor->GetFunctionOverload();
		}


		if (!tilda)
		{
			//ReasonPtr ctorReason = Reason::Create(CppAxioms::CtorCreation(),
			//									  itr.GetSource());
			//ConstructorPtr ctor = Constructor::Create(ctorNode, ctorReason);
			////isInline, access,
			//ConstructorOverloadPtr ctorOl =
			//	ConstructorOverload::Create(ctor, isInline, access, ctorReason);
			//fOlPtr = boost::dynamic_pointer_cast<FunctionOverload>(ctorOl);

			ConsumeWhitespace(newItr);
			if (newItr.ConsumeChar(':'))
			{
				// Look for assignments to class vars, such as x(4), y(3)
				do
				{
					ConsumeWhitespace(newItr);
					std::string varName;
					if (!ConsumeComplexName(newItr, varName))
					{
						throw ParserException(newItr.GetSource(),
							Messages::Get("CppParser.Constructor.VariableInitializer"));
					}

					NodePtr varNode = currentScope->FindOrCreate(varName);

					ConsumeWhitespace(newItr);

					if (!newItr.ConsumeChar('('))
					{
						throw ParserException(newItr.GetSource(),
							Messages::Get("CppParser.Constructor.NoInitializerExpr"));
					}

					std::string exprCode;
					ConsumeExpression(newItr, ")", exprCode);
					newItr.Advance(1);

					VariableAssignment va;
					va.Expression = exprCode;
					va.Variable = varNode;
					ConstructorOverloadPtr ctorOl =
						boost::dynamic_pointer_cast<ConstructorOverload>(fOlPtr);
					ctorOl->AddAssignment(va);

					ConsumeWhitespace(newItr);
				} while(newItr.ConsumeChar(','));
			}
		} // end !tilda

		FunctionCodeDefInfo defInfo(*this, newItr);
		defInfo.ApplyToFunction(fOlPtr);

		itr = newItr;
		return true;
	}

	bool ConsumeComment(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		if (itr.Finished() || itr.Current() != '/')
		{
			return false;
		}
		Iterator cmtItr = itr;
		cmtItr.Advance(1);
		if (cmtItr.Finished())
		{
			return false;
		}
		if (cmtItr.Current() == '/')
		{
			itr.Advance(1);
			while(!itr.Finished() && itr.Current() != '\n')
			{
				itr.Advance(1);
			}
			return true;
		}
		else if (cmtItr.Current() == '*')
		{
			itr.Advance(1);
			bool seenStar = false;
			bool commentFinished = false;
			while(!itr.Finished() && !commentFinished)
			{
				if (seenStar && itr.Current() == '/')
				{
					commentFinished = true;
				}
				if (itr.Current() == '*')
				{
					seenStar = true;
				}
				else
				{
					seenStar = false;
				}
				itr.Advance(1);
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	void ConsumeWhitespace(Iterator & itr)
	{
		while(!itr.Finished() && ConsumeComment(itr)){}
	}

	/** If no complex name found, nothing happens.
	 * If one is found, it is consumed and the name itself is placed in result.
	 */
	static bool ConsumeComplexName(Iterator & itr, std::string & result)
	{
		int endWhiteSpaceOfKeyword;
		int length = ComplexName(itr, endWhiteSpaceOfKeyword);
		if (length < 1)
		{
			return false;
		}
		std::stringstream ss;
		for(int i = 0; i < length; i ++)
		{
			// operator overloads may put spaces here...
			// For the purposes of Macaroni generators, the canonical version
			// should be used.
			if (!itr.IsWhiteSpace())
			{
				ss << itr.Current();
			}
			if (i == endWhiteSpaceOfKeyword)
			{
				// Put a *single* whitespace in the canonical name of
				// a type with the signed or unsigned keyword.
				ss << " ";
			}
			itr.Advance(1);
		}
		result = ss.str();
		return true;
	}

	/** Reads through an arbitrary block of code, stopping at one of the provided
	 *  chars.  StopAtChars must be terminated with \0.
	 *  Does NOT return on ) or } if these close a block; for example, you
	 *  must push the iterator to past the first { if you plan on stopping at
	 *  }, or else the parser will simply think the { is new and increase
	 *  its depth.
	 **/
	void ConsumeExpression(Iterator & itr, const char * stopAtChars,
		                   std::string & code, bool forTrailingReturnType=false)
	{
		Iterator exceptionItr = itr;

		std::vector<char> blocks;
		std::stringstream ss;

		while(!itr.Finished())
		{
			char c = itr.Current();
			if (c == '(')
			{
				blocks.push_back('(');
			}
			// Allow a new open brace but only if the block size has already
			// gone past zero due to something like an open paranthesis.
			else if ((!forTrailingReturnType || blocks.size() > 0)
				     && c == '{')
			{
				blocks.push_back('{');
			}
			else if (	blocks.size() > 0 &&
						(
							(blocks.back() == '(' && c == ')')
							||
							(blocks.back() == '{' && c == '}')
						)
					)
			{
				blocks.pop_back();
			}
			else if (forTrailingReturnType && blocks.size() > 0)
			{
				// Never let this get past here if the block size is greater
				// than zero; otherwise certain lambdas can't be put into
				// decltypes in the trailing return type.
				// TODO: Maybe this option should be in place for everything?
			}
			else
			{
				for (int stopAtI = 0; stopAtChars[stopAtI] != '\0'; stopAtI ++)
				{
					if (c == stopAtChars[stopAtI])
					{
						code = ss.str();
						//itr.Advance(1); //2010-01-03 Changed to not advance past char.
						return;
					}
				}
			}

			ss << c;
			itr.Advance(1);
		}

		throw ParserException(exceptionItr.GetSource(),
			Messages::Get("CppParser.Expression.CouldNotMatch"));
	}

	bool ConsumeFilePath(Iterator & itr, std::string & filePath)
	{
		std::stringstream ss;
		bool ankles = false;
		if (itr.Current() == '<')
		{
			ankles = true;
		}
		else if (!(itr.Current() == '"'))
		{
			return false;
		}
		ss << itr.Current();
		itr.Advance(1);

		while(!itr.Finished() && itr.Current() !='\"' && itr.Current() !='>')
		{
			ss << itr.Current();
			itr.Advance(1);
		}

		if (itr.Finished())
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.FileNameExpected"));
		}
		ss << itr.Current();

		//TO-DO: Check for > if ankels is true and " if not true, and make sure
		// there's no mismatch.

		itr.Advance(1);
		filePath = ss.str();
		return true;
	}

	/** Attempts to consume a name, and then stores the Node by that name.
	 * Returns false if either no complex name was there, and sets node to nullptr
	 * if it can't find a node with that name.
	 * IOW, be aware this may return true but not have found a node. */
	bool ConsumeNodeName(Iterator & itr, NodePtr & node)
	{
		node = NodePtr();
		Iterator newItr = itr;
		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			return false;
		}
		node = FindNode(name);
		if (!!node)
		{
			// Advance only if Node was found.
			itr = newItr;
		}
		return true;
	}

	/** If no simple name found, nothing happens.
	 * If one is found, it is consumed and the name itself is placed in result.
	 * Btw, if this finds a Complex name it stops right at :: and returns
	 * true, so be prepared for that sucka.
	 */
	static bool ConsumeSimpleName(Iterator & itr, std::string & result)
	{
		int length = SimpleName(itr);
		if (length < 1)
		{
			return false;
		}
		std::stringstream ss;
		for(int i = 0; i < length; i ++)
		{
			ss << itr.Current();
			itr.Advance(1);
		}
		result = ss.str();
		return true;
	}

	/** Searches for "anything". Will move itr, and places result in rtnString. */
	bool ConsumeStringLiteral(Iterator & itr, std::string & rtnString)
	{
		std::stringstream ss;
		if (!(itr.Current() == '"'))
		{
			return false;
		}
		itr.Advance(1); // skip first "

		while(!itr.Finished() && itr.Current() !='\"')
		{
			ss << itr.Current();
			itr.Advance(1);
		}

		if (itr.Finished())
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.StringLiteralEndingExpected"));
		}
		itr.Advance(1); // skip last "
		rtnString = ss.str();
		return true;
	}

	/** Looks for a node name with type arguments either at the end of in the middle.
	 * Example: a::b<c>, a::b::d, a<b,c>::b<a::b::c>::e<a *,const e::g>
	 * Returns false if it does not find a name.
	 */
	bool ConsumeTypeMainNodeAndArguments(Iterator & itr, TypePtr & type)
	{
		NodePtr mainNode;
		NodePtr lastNode;

		while(!mainNode)
		{
			ConsumeWhitespace(itr);
			std::string complexName;
			Iterator beforeNameItr = itr;
			if (!ConsumeComplexName(itr, complexName))
			{
				return false;
			}
			ConsumeWhitespace(itr);

			NodePtr node;
			if (!lastNode) // The first iteration of the loop?
			{
				node = FindNode(complexName);
				if (!node)
				{
					// At this point, we're committed, so start using the actual
					// itr passed in.
					throw ParserException(beforeNameItr.GetSource(),
						Messages::Get("CppParser.Variable.UnknownTypeName"));
				}
			}
			else
			{
				// If we've already seen a node (and a type arg list) then the
				// main node has to come off of it.
				node = lastNode->FindOrCreate(complexName);
			}

			if (!type)
			{
				type = context->CreateType();
			}


			if (itr.Current() == '<')
			{
				itr.ConsumeChar('<');
				TypeArgumentListPtr argumentList
					= type->AddArgument(node->GetDepth());
				ConsumeTypeDefinitionList(itr, argumentList);

				itr.ConsumeWhitespace();
				if (itr.ConsumeWord("::")) // More to come, looks like.
				{
					lastNode = node;
					continue; // SO tempted to just use a GOTO here...
					          // too lazy to decompose method.
				}
			}

			mainNode = node;
		}

		type->SetNode(mainNode);

		return true;
	}



	/** This is called to find lists of type defs, as can be seen in ankle
	 * brackets.  Throws if it can't find type argument.   Ends when it
	 * sees ">". */
	void ConsumeTypeDefinitionList(Iterator & itr,
		                           TypeArgumentListPtr argList)
	{
		while(true) // Remember, its not as bad as GOTO because its WHILE. :p
		{
			Iterator newItr = itr;
			TypePtr type = argList->CreateType();
			if (!Type_(newItr, type))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Type.TypeDefinitionArgumentExpected"));
			}
			itr = newItr;
			ConsumeWhitespace(itr);
			if (itr.Current() == '>')
			{
				itr.ConsumeChar('>');
				return;
			}
			else if (itr.Current() == ',')
			{
				itr.ConsumeChar(',');
				// Continue loop.
			}
			else
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Type.TypeDefinitionCommaOrClosingBracketExpected"));
			}
			type->Lock();
		}
	}

	bool ConstExprKeyword(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		return itr.ConsumeWord("constexpr");
	}

	bool ConstKeyword(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		return itr.ConsumeWord("const");
	}

	NodePtr createNextBlockNode(NodePtr node, Iterator & itr)
	{
		const int maxBlocks = 1024;
		if (!node)
		{
			throw Macaroni::StringException("This function does not accept a null node.");
		}
		for(int index = 0; index < maxBlocks; index ++)
		{

			std::stringstream ss;
			ss << "~#block-";
			ss << currentTarget->GetName();
			ss << "-";
			ss << index;
			std::string name = ss.str();
			NodePtr next = node->Find(name);
			if (!next)
			{
				return node->FindOrCreate(name);
			}
		}
		std::stringstream ss;
		ss << "The node " << node->GetFullName()
			<< " has exceded the maximum number of blocks allowed by the "
			<< "Parser (" << maxBlocks << ").";
		throw ParserException(itr.GetSource(), ss.str());
	}

	// Given some node - such as a class or function - create a new
	// Unit. This replaces the old Porg (Physical Organizer) plugin.
	// Returns new target.
	TargetPtr createImplicitUnitForNode(Node & node)
	{
		std::string tName = node.GetPrettyFullName("/");
		auto_ptr<UnitTarget> utp(UnitTarget::CreateImplicit(
			currentTarget, true, tName));
		utp->SetHFileAsUnknownRelativePath(tName + ".h");
		utp->SetCppFileAsUnknownRelativePath(tName + ".cpp");
		TargetPtr rtn(utp.get());
		utp.release();
		return rtn;
	}

	// True if the current target is a library. When the parser starts,
	// this is true, but if it sees the ~unit keyword or creates a unit after
	// seeing a class it changes.
	bool currentTargetIsLibrary()
	{
		return (dynamic_cast<LibraryTarget *>(currentTarget.get()) != nullptr);
	}

	bool DeclTypeAutoHack(Iterator & itr, TypePtr & type)
	{
		if (!itr.ConsumeWord("decltype(auto)"))
		{
			return false;
		}
		NodePtr mainNode = CppContext::GetPrimitives(context)
		                   ->Find("decltype(auto)");
		MACARONI_ASSERT(nullptr != mainNode.get(),
			            "decltype(auto) node not found!");
		if (!type)
		{
			type = context->CreateType();
		}
		type->SetNode(mainNode);
		return true;
	}

	/** Determines where an element should live.
	 *  If target was not passed in, an empty ptr is returned and "library"
	 *  should be used instead.
	 *  Otherwise, if target was passed in but the current target has been
	 *  changed manually (using "~target" or something like that) then the
	 *  current target is used. If the target has not been changed manually
	 *  then a new UnitTarget is created for each unique element. */
	TargetPtr deduceTargetHome(NodePtr & node)
	{
		if (currentTarget)
		{
			return currentTarget;
		}
		else
		{
			MACARONI_ASSERT(defaultTarget, "Default Target null in Parser!");
			return defaultTarget;
		}
	}

	bool Directives(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);
		if (newItr.Finished() || !(newItr.Current() == '~'))
		{
			return false;
		}
		newItr.Advance(1);
		if (!Import(newItr))
		{
			return false;
			// For awhile now, ~ does not necessarily indicate a directive,
			// but a keyword unique to Macaroni.
			// So we certainly don't want to throw...
			//throw ParserException(itr.GetSource(),
			//	Messages::Get("CppParser.Directive.Unknown"));
		}
		itr = newItr;
		return true;
	}

	/** The start of a doc.  Either it reads stuff in or throws an exception. */
	void Document(Iterator itr)
	{
		ScopeFiller(itr);
		ConsumeWhitespace(itr);
		if (!itr.Finished())
		{
			throw ParserException(itr.GetSource(), Messages::Get("CppParser.Document.SyntaxError"));
		}
	}

	bool Enum(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);

		AccessPtr access = AccessKeyword(newItr);
		if (*access == *Access::NotSpecified())
		{
			access = currentAccess;
		}
		else
		{
			newItr.ConsumeWhitespace();
		}

		if (!newItr.ConsumeWord("enum"))
		{
			return false;
		}
		ConsumeWhitespace(newItr);

		const bool isClass = newItr.ConsumeWord("class");
		ConsumeWhitespace(newItr);

		std::string nodeName;
		if (!ConsumeComplexName(newItr, nodeName))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Enum.NoID1"));
		}
		ConsumeWhitespace(newItr);

		TypePtr sizeType;
		if (newItr.ConsumeChar(':'))
		{
			ConsumeWhitespace(newItr);
			if (!Type_(newItr, sizeType))
			{
				throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Enum.Size.NoType"));
			}
			ConsumeWhitespace(newItr);
		}

		NodePtr oldScope = currentScope;
		currentScope = currentScope->FindOrCreate(nodeName);

		TargetPtr tHome = deduceTargetHome(currentScope);
		auto ePtr = Enum::Create(tHome, currentScope,
								 Reason::Create(CppAxioms::EnumCreation(),
								 	            newItr.GetSource()),
								 isClass, sizeType, *access);

		// Parse annotations.
		while(Annotation(newItr));

		if (!newItr.ConsumeChar('{'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Enum.NoStartBrace"));
		}
		ConsumeWhitespace(newItr);

		// Parse the body of the enum.

		std::string name;
		std::string value;
		bool seenOne = false;
		while(!newItr.ConsumeChar('}'))
		{
			if (seenOne)
			{
				if (!newItr.ConsumeChar(','))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Enum.NoComma"));
				}
			}
			ConsumeWhitespace(newItr);
			if (!ConsumeSimpleName(newItr, name))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Enum.NoValueName"));
			}
			ConsumeWhitespace(newItr);
			if (newItr.ConsumeChar('='))
			{
				ConsumeExpression(newItr, " ,}\n\r", value);
				if (value.length() < 1)
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Enum.NoValueFollowingEquals"));
				}
				ConsumeWhitespace(newItr);
				ePtr->AddValue(name, value);
			}
			else
			{
				ePtr->AddValue(name);
			}
			seenOne=true;
		}
		ConsumeWhitespace(newItr);
		if (!newItr.ConsumeChar(';'))
		{
			throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Enum.NoSemicolon"));
		}

		itr = newItr;
		currentScope = oldScope;
		return true;
	}

	// Helper for extern, sets a field value (i.e. like "using={blah}").
	void externField(Iterator & itr,
		             boost::optional<std::string> & fieldValue,
		             bool isFilePath=false)
	{
		if (fieldValue) {
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Extern.PropertyAlreadySet"));
		}
		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar('=')) {
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Extern.ExpectedEquals"));
		}
		if (isFilePath) {
			//TODO: Would it ever make sense to allow the include to be
			//      arbitrary text? Just in case do that part here.
			std::string filePath;
			if (!ConsumeFilePath(itr, filePath))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Extern.MissingFilePath"));
			}
			fieldValue = filePath;
		} else {
			std::string codeBlock;
			SourcePtr src; // Ignored
			if (!CodeBlock(itr, codeBlock, src)) {
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Extern.MissingCodeBlock"));
			}
			fieldValue = codeBlock;
		}
	}

	/* Looks for "~extern " and creates an "extern" element. Modifies itr
	 * if match is found. */
	bool Extern(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		Iterator startItr = itr;
		if (!itr.ConsumeWord("~extern"))
		{
			return false;
		}

		ConsumeWhitespace(itr);
		std::string name;
		if (!ConsumeComplexName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Extern.NoID1"));
		}

  		boost::optional<std::string> usingS;
    	boost::optional<std::string> forward;
    	boost::optional<std::string> include;

    	bool exportThis = false;
		while(true)
		{
			ConsumeWhitespace(itr);
			if (itr.ConsumeWord("using")) {
				externField(itr, usingS);
				if (usingS
					&& std::all_of(usingS.get().begin(), usingS.get().end(),
						           [](unsigned char c){
						           	  return std::isspace(c);
						           }))
				{
					// For reasons that are hard to get into (i.e. laziness)
					// sometimes generated code may have empty using statements.
					// In these cases just set it to boost::none.
					usingS = boost::none;
				}
			} else if (itr.ConsumeWord("forward")) {
				externField(itr, forward);
			} else if (itr.ConsumeWord("include")) {
				externField(itr, include, true);
		 	} else if (itr.ConsumeWord("export")) {
		 		exportThis = true;
			} else if (itr.ConsumeChar(';')) {
				break;
			} else {
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Extern.SyntaxError"));
			}
		}

		NodePtr home = currentScope->FindOrCreate(name);

		TargetPtr tHome;
		if (exportThis)
		{
			tHome = deduceTargetHome(currentScope);
		}
		Extern::Create(
			tHome,
			home,
			Reason::Create(CppAxioms::ExternCreation(), startItr.GetSource()),
			usingS, forward, include
		);
		return true;
	}

	/** Given a Node, crawls up the Node tree to find the first
	 * parent (or the given node itself) which is a Class. */
	NodePtr FindClassAncestorOfNode(NodePtr node)
	{
		if (!node)
		{
			return NodePtr();
		}
		if (Class::IsInstance(node->GetElement()))
		{
			return node;
		}
		return FindClassAncestorOfNode(node->GetNode());
	}

	/** Given a Node, crawls up the Node tree to find the first
	 * parent (or the given node itself) which is a Namespace. */
	NodePtr FindNamespaceAncestorOfNode(NodePtr node)
	{
		if (!node)
		{
			return NodePtr();
		}
		if (Namespace::IsInstance(node->GetElement()))
		{
			return node;
		}
		return FindNamespaceAncestorOfNode(node->GetNode());
	}

	/** Takes a complex name and finds its node. */
	NodePtr FindNode(const std::string & complexName)
	{
		/*
		// The Node class really needs a routine to discover a name given the
		// current scope.  The visibility rules are too complex to
		// lay out here.

		// TO-DO: Create method in Node class called "FindNodeUsingVisibilityRules"
		// For now, just do this:
		NodePtr result;
		result = currentScope->Find(complexName);
		if (!result)
		{
			result = FindNodeFromImports(complexName);
		}
		if (!result)
		{
			if (complexName == currentScope->GetFullName() ||
				complexName == currentScope->GetName())
			{
				return currentScope;
			}
		}
		return result;*/
		NodePtr rtnValue;
		rtnValue = FindNodeFromTemplateParameters(complexName);
		if (!rtnValue)
		{
			rtnValue = FindNodeFromScope(complexName);
		}
		return rtnValue;
	}

	NodePtr FindNodeFromImports(const std::string & complexName)
	{
		std::string firstPart, lastPart;
		Node::SplitFirstNameOffComplexName(complexName, firstPart, lastPart);
		for (unsigned int i = 0 ; i < currentImports().size(); i ++)
		{
			auto & imp = currentImports()[i];
			//NodePtr & imp = (*currentImports)[i];
			if (firstPart == imp.GetNode().GetName())
			{
				if (lastPart.size() < 1)
				{
					return &(imp.GetNode());
				}
				return imp.GetNode().Find(lastPart);
			}
		}
		return NodePtr();
	}

	NodePtr FindNodeFromImportsOrScope(const std::string & complexName)
	{
		NodePtr rtnValue = FindNodeFromImports(complexName);
		NodePtr scopeItr = currentScope;
		if (complexName == currentScope->GetName() ||
			complexName == currentScope->GetFullName())
		{
			return currentScope;
		}
		while (!rtnValue && !!scopeItr)
		{
			rtnValue = scopeItr->Find(complexName);
			scopeItr = scopeItr->GetNode();
		}
		return rtnValue;
	}

	/** Finds a node using visibility rules.
	 * First checks the Class, then the namespace, and finally imports.
	 */
	NodePtr FindNodeFromScope(const std::string & complexName)
	{
		NodePtr scope = currentScope;
		NodePtr classNode = FindClassAncestorOfNode(scope);
		NodePtr namespaceNode = FindNamespaceAncestorOfNode(scope);

		NodePtr rtn;
		// Check class first,
		rtn = FindNodeFromNode(classNode, complexName);
		// then namespace,
		if (!rtn)
		{
			rtn = FindNodeFromNode(namespaceNode, complexName);
			if (!rtn)
			{
				rtn = FindNodeFromImports(complexName);
			}
		}
		if (!rtn)
		{
			// 2010-03-28 - Adding this code may break something as I haven't
			// been thinking about this section lately but I think this will
			// fix it so you can use long names for Nodes...
			rtn = this->context->GetRoot()->Find(complexName);
		}
		return rtn;
	}

	/** Searchs a node for the given complex name. */
	NodePtr FindNodeFromNode(NodePtr scope, const std::string & complexName)
	{
		if (!scope)
		{
			return NodePtr();
		}
		NodePtr scopeItr = scope;
		if (complexName == scope->GetName() ||
			complexName == scope->GetFullName())
		{
			return scope;
		}
		if (!!scope->GetAdoptedHome())
		{
			if (complexName == scope->GetAdoptedHome()->GetName() &&
				complexName == scope->GetAdoptedHome()->GetFullName())
			{
				return scope->GetAdoptedHome();
			}
		}
		NodePtr rtnValue;
		while (!rtnValue && !!scopeItr)
		{
			rtnValue = scopeItr->Find(complexName);
			scopeItr = scopeItr->GetNode();
		}
		return rtnValue;
	}

	NodePtr FindNodeFromTemplateParameters(const std::string & complexName)
	{
		NodePtr rtnValue;
		for (int i = this->currentTemplateParameters.size() - 1;
			 i >= 0; --i)
		{
			NodePtr & tN = this->currentTemplateParameters[i];
			rtnValue = tN->Find(complexName);
			if (rtnValue)
			{
				break;
			}
		}
		return rtnValue;
	}


	static void FindNodeTest()
	{
		ContextPtr c(new Context("{ROOT}"));
		CppContext::CreateCppNodes(c);

		TargetPtr t;
		ParserFunctions funcs(c, t);

		// It is found.
		NodePtr nodeInt = funcs.FindNode("signed int");
		Assert(nodeInt->GetFullName() == CppContext::GetPrimitives(c)->Find("signed int")->GetFullName());

		// int is found, but nothing is found beyond that.
		NodePtr nodeInt2 = funcs.FindNode("signed int::something");
		Assert(nodeInt2 == NodePtr());

		NodePtr seed = c->GetRoot()->FindOrCreate(std::string("Fruit::Orange::Seed"));
		NodePtr orange = seed->GetNode();
		NodePtr fruit = orange->GetNode();
		funcs.AddImport(ImportType::Normal, orange);
		NodePtr foundSeed = funcs.FindNode(std::string("Orange::Seed"));
		Assert(foundSeed->GetFullName() == seed->GetFullName());

		NodePtr foundFruit = funcs.FindNode(std::string("Fruit"));
		Assert(foundFruit->GetFullName() == fruit->GetFullName());
	}

	static void FindNodeFromImportsTest()
	{
		ContextPtr c(new Context("{ROOT}"));
		CppContext::CreateCppNodes(c);

		TargetPtr t;
		ParserFunctions funcs(c, t);

		// It is found.
		NodePtr nodeInt = funcs.FindNodeFromImports(std::string("signed int"));
		Assert(nodeInt->GetFullName() == CppContext::GetPrimitives(c)->Find("signed int")->GetFullName());

		// int is found, but nothing is found beyond that.
		NodePtr nodeInt2 = funcs.FindNodeFromImports(std::string("signed int::something"));
		Assert(nodeInt2 == NodePtr());

		NodePtr orange = c->GetRoot()->FindOrCreate(std::string("Fruit::Orange"));
		NodePtr fruit = orange->GetNode();
		funcs.AddImport(ImportType::Normal, fruit);
		NodePtr foundOrange = funcs.FindNodeFromImports(std::string("Fruit::Orange"));
		Assert(foundOrange->GetFullName() == orange->GetFullName());
	}

	bool FriendDeclaration(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("friend"))
		{
			return false;
		}

		ClassPtr classPtr = currentScope->GetElement<ClassPtr>();
		if (!classPtr) //!!currentScope->GetElement())
		{
		//	/*classPtr = boost::dynamic_pointer_cast<Macaroni::Model::Cpp::Class>(currentScope->GetElement());
		//}
		//if (!classPtr)
		//{*/
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.FriendKeywordOnlyForClasses"));
		}

		ConsumeWhitespace(itr);
		std::string friendName;
		ConsumeComplexName(itr, friendName);
		NodePtr friendNode = FindNodeFromImportsOrScope(friendName);
		if (!friendNode)
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.CouldNotFindNode"));
		}
		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Friend.MissingSemicolon"));
		}
		classPtr->AddFriend(friendNode);
		return true;
	}

	/** Attempts to consume "~friend". Moves itr. */
	bool FriendModifierKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("~friend");
	}

	/** This function expects us to be committed to finding a function and to
	 * have seen '('. We parse until we see ')'. */
	void FunctionArgumentList(Iterator & itr)//, TypeInfo & rtnTypeInfo,
							  //std::string & name)
	{
		ConsumeWhitespace(itr);
		bool seenArg = false;
		while(!itr.ConsumeChar(')'))
		{
			Iterator oldItr = itr;
			AccessPtr access = Access::NotSpecified();
			bool _friend;
			NodePtr global;
			bool isConstExpr;
			bool isInline;
			std::string initializer;
			bool isStatic;
			bool isVirtual;
			TypePtr type;
			std::string argName;
			if ((seenArg && !itr.ConsumeChar(','))
				||
				!Variable(itr, access, _friend, global, isInline, isStatic, isVirtual, isConstExpr, type, argName))
			{
				throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Function.ExpectedEndingParenthesis"));
			}
			itr.ConsumeWhitespace();
			if (itr.ConsumeChar('='))
			{
				ConsumeExpression(itr, ",)", initializer);
			}
			if (_friend)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.FriendNotAllowedForArg");
			}
			if (!!global)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.GlobalNotAllowedForArg");
			}
			if (isInline)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.InlineNotAllowedForVariable");
			}
			if (isStatic)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.StaticNotAllowedForArg");
			}
			if (isVirtual)
			{
				throw ParserException(oldItr.GetSource(),
					"CppParser.Variable.VirtualNotAllowedForArg");
			}

			NodePtr node = currentScope->FindOrCreate(argName);
			TargetPtr tHome;
			Variable::Create(tHome, node, access, isStatic, isConstExpr, type, initializer,
				Reason::Create(CppAxioms::VariableScopeCreation(), oldItr.GetSource()));
			seenArg = true;
			ConsumeWhitespace(itr);
		}
	}

	struct FunctionCodeDefInfo
	{
		std::string codeBlock;
		bool pureVirtual;
		bool defaultKeyword;
		bool deleteKeyword;
		bool codeAttached;
		SourcePtr startOfCodeBlock;

		FunctionCodeDefInfo(ParserFunctions & parent, Iterator & itr)
		:	codeBlock(),
			pureVirtual(false),
			defaultKeyword(false),
			deleteKeyword(false),
			codeAttached(false),
			startOfCodeBlock()
		{
			codeAttached = parent.CodeBlock(itr, codeBlock, startOfCodeBlock);
			if (!codeAttached)
			{
				if (!(parent.PureVirtualOrDefaultKeyword(
						pureVirtual, defaultKeyword, deleteKeyword, itr,
						startOfCodeBlock
					)))
				{
					parent.ConsumeWhitespace(itr);
					if (!itr.ConsumeChar(';'))
					{
						throw ParserException(itr.GetSource(),
							Messages::Get("CppParser.Function.SemicolonExpected"));
					}
				}
			}
		}

		void ApplyToFunction(FunctionOverloadPtr fOl)
		{
			if (codeAttached)
			{
				fOl->SetCodeBlock(codeBlock, startOfCodeBlock, true);
			}
			else if (pureVirtual)
			{
				fOl->SetPureVirtual(startOfCodeBlock);
			}
			else if (defaultKeyword)
			{
				fOl->SetDefault(startOfCodeBlock);
			}
			else if (deleteKeyword)
			{
				fOl->SetDelete(startOfCodeBlock);
			}
		}
	};


	bool GlobalKeyword(Iterator & itr, NodePtr & home)
	{
		home.reset();

		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("~global"))
		{
			return false;
		}
		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar('('))
		{
			home = currentScope->GetNode();
			return true;
		}
		itr.ConsumeWhitespace();
		if (!ConsumeNodeName(itr, home))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.GlobalKeyword.HomeNodeExpected"));
		}
		if (!home)
		{
			// No node was found? Check to see if the root node was desired.
			// I thought of adding to Node::FindNode the ability to specify
			// the root namespace with "::". But thinking about that, it would
			// make it legal to identify say node A::B::C with A::B::C::. The
			// trailing "::" seems too gross... maybe in time though that
			// idea will make sense.
			if (!itr.ConsumeWord("::"))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.GlobalKeyword.HomeNodeExpected"));
			}
			home = currentScope->GetContext()->GetRoot();
		}
		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar(')'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.GlobalKeyword.ClosingParanthesisExpected"));
		}
		return true;
	}

	bool HFileDirective(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("~hfile"))
		{
			return false;
		}
		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar('='))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.HFileBadArgument"));
		}
		ConsumeWhitespace(itr);
		std::string filePath;
		if (!ConsumeFilePath(itr, filePath))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Directive.HFileFilePathExpected"));
		}
		FileNamePtr fileName = FileName::Create("", filePath);
		ReasonPtr reason = Reason::Create(CppAxioms::SetExistingHFilePath(), itr.GetSource());
		this->currentScope->SetHFilePath(reason, fileName);
		return true;
	}

	// looks for "import [complexName];"
	// Modifies itr argument if match is found.
	bool Import(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("import"))
		{
			return false;
		}

		const bool hImport = itr.ConsumeWord("-h");

		// Now we're committed!
		ConsumeWhitespace(itr);

		std::string name;
		if (!ConsumeComplexName(itr, name))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.NameNotFound"));
		}

		ConsumeWhitespace(itr);
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Import.SemicolonExpected"));
		}

		NodePtr importNode = context->GetRoot()->FindOrCreate(name);
		AddImport(hImport ? ImportType::H : ImportType::Normal, importNode);
		return true;
	}

	ImportList & currentImports()
	{
		return *(importedNodesStack.back());
	}

	bool InlineKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("inline");
	}

	// looks for "namespace [complexName]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Namespace(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);

		bool withBraces = true;

		if (newItr.ConsumeWord("namespace"))
		{
			withBraces = true;
		}
		else if (newItr.ConsumeWord("~namespace"))
		{
			withBraces = false;
		}
		else
		{
			return false;
		}

		// Once the parser has seen "namespace" it must see the correct
		// grammar or there will be hell to pay.

		ConsumeWhitespace(newItr);

		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Namespace.NoID1"));
		}

		NodePtr oldScope = currentScope;
		currentScope = currentScope->FindOrCreate(name);

		// Create any parent namespaces that may have been introduced;
		// for example, if we parse "A::B", A is also a namespace
		// because only Namespaces can contain Namespaces.
		NodePtr ns = currentScope;
		while(ns != oldScope)
		{
			if (!ns->GetElement())
			{
				TargetPtr tHome = deduceTargetHome(ns);
				Namespace::Create(tHome, ns,
						Reason::Create(CppAxioms::NamespaceCreation(),
						               newItr.GetSource()));
			}
			ns = ns->GetNode();
		}

		ConsumeWhitespace(newItr);

		SourcePtr firstBraceSrc = itr.GetSource();

		if (withBraces)
		{
			if (!newItr.ConsumeChar('{'))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Namespace.NoOpeningBrace"));
				//EXCEPTION MSG: Expectedd { after namespace identifier.")
				return false;
			}
		}
		else
		{
			if (!newItr.ConsumeChar(';'))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Namespace.NoOpeningBrace"));
				//EXCEPTION MSG: Expectedd { after namespace identifier.")
				return false;
			}
		}

		ScopeFiller(newItr);

		ConsumeWhitespace(newItr);

		//Model::Cpp::Namespace::Create(currentScope,
		//
	///		);

		if (withBraces)
		{
			if (!newItr.ConsumeChar('}'))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Namespace.NoEndingBrace", firstBraceSrc->GetLine()));
			}
		}
		else
		{
			if (!newItr.Finished())
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Namespace.StatementEof",
					newItr.GetSource()->GetLine()));
			}
		}
		currentScope = oldScope;
		itr = newItr; // Success! :)
		return true;
	}

	// Parses "operator blah", where blah is the operator being overloaded.
	// Its necessary to do it this way because "operatorFunctions" is a valid
	// name but "operator+" refers to "operator +".  So we have to actually
	// check the operator and the keyword here.
	static int OperatorName(Iterator & itr, int & endWhiteSpaceOfKeyword)
	{
		Iterator myItr = itr;
		if (!myItr.ConsumeWord("operator"))
		{
			return 0;
		}
		int consumed = 8;
		bool committed = myItr.IsWhiteSpace();
		consumed += myItr.ConsumeWhitespace();
		if (OverloadableOperator(myItr, consumed, endWhiteSpaceOfKeyword))
		{
			return consumed;
		}
		else if (committed)
		{
			throw ParserException(myItr.GetSource(),
				Messages::Get("CppParser.Operator.InvalidOperator"));
		}
		else
		{
			return 0;
		}
	}

	// If the itr lies at an overloadable operator, parses through it and
	// returns the consumed count, then returns true.  Otherwise returns false.
	static bool OverloadableOperator(Iterator & itr, int & consumed,
		                             int & endWhiteSpaceOfKeyword)
	{
		endWhiteSpaceOfKeyword = -1;
		int consume = 0;
		if (itr.ConsumeWord("new[]"))
		{
			endWhiteSpaceOfKeyword = consumed - 1;
			consume = 5;
		}
		else if (itr.ConsumeWord("new"))
		{
			endWhiteSpaceOfKeyword = consumed - 1;
			consume = 3;
		}
		else if (itr.ConsumeWord("delete[]"))
		{
			endWhiteSpaceOfKeyword = consumed - 1;
			consume = 8;
		}
		else if (itr.ConsumeWord("delete"))
		{
			endWhiteSpaceOfKeyword = consumed - 1;
			consume = 6;
		}
		else if (itr.ConsumeWord("[]")
			|| itr.ConsumeWord("->")
			|| itr.ConsumeWord("==")
			|| itr.ConsumeWord("!=")
			|| itr.ConsumeWord(">=")
			|| itr.ConsumeWord("<=")
			|| itr.ConsumeWord("&&")
			|| itr.ConsumeWord("||")
			|| itr.ConsumeWord("++")
			|| itr.ConsumeWord("--")
			|| itr.ConsumeWord("+=")
			|| itr.ConsumeWord("-=")
			|| itr.ConsumeWord("*=")
			|| itr.ConsumeWord("/=")
			|| itr.ConsumeWord("<<")
			|| itr.ConsumeWord(">>")
			|| itr.ConsumeWord("()"))
		{
			consume = 2;
		}
		else if (itr.ConsumeChar('+')
			|| itr.ConsumeChar('-')
			|| itr.ConsumeChar('*')
			|| itr.ConsumeChar('/')
			|| itr.ConsumeChar('%')
			|| itr.ConsumeChar('^')
			|| itr.ConsumeChar('|')
			|| itr.ConsumeChar('&')
			|| itr.ConsumeChar('&')
			|| itr.ConsumeChar('~')
			|| itr.ConsumeChar('<')
			|| itr.ConsumeChar('>')
			|| itr.ConsumeChar('!')
			|| itr.ConsumeChar('=')
			)
		{
			consume = 1;
		}
		else
		{
			return false;
		}
		consumed += consume;
		return true;
	}

	bool OverrideKeyword(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		return itr.ConsumeWord("override");
	}

	/** Determines what exception specifier, if any, exists here. */
	boost::optional<ExceptionSpecifier> ParseExceptionSpecifier(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		if (itr.ConsumeWord("throw"))
		{
			itr.ConsumeWhitespace();
			if (!itr.ConsumeChar('('))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.ThrowSpecifier.MissingFirstParanthesis"));
			}
			//TODO: If for some reason the ability to put a type specifier into the
			//throw statement is ever worth doing, remember to change the error
			//message which currently gives a reminder that it isn't.
			itr.ConsumeWhitespace();
			if (!itr.ConsumeChar(')'))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.ThrowSpecifier.MissingSecondParanthesis"));
			}
			return ExceptionSpecifier::EmptyThrows();
		}
		else if (itr.ConsumeWord("BOOST_NOEXCEPT"))
		{
			return ExceptionSpecifier::BoostNoExcept();
		}
		else if (itr.ConsumeWord("noexcept"))
		{
			itr.ConsumeWhitespace();
			if (!itr.ConsumeChar('('))
			{
				return ExceptionSpecifier::NoExcept();
			}
			std::string expr;
			ConsumeExpression(itr, ")", expr);
			//TODO: If for some reason the ability to put a type specifier into the
			//throw statement is ever worth doing, remember to change the error
			//message which currently gives a reminder that it isn't.
			return ExceptionSpecifier::NoExcept(expr);
		}
		else
		{
			return boost::none;
		}
	}

	/** Returns true if the pure virtual syntax is found here (in other words,
	 *  "= 0;"). "source" is the start of the pure virtual expression.
	 */
	bool PureVirtualOrDefaultKeyword(bool & virtualKeyword,
		                             bool & defaultKeyword,
		                             bool & deleteKeyword,
		                             Iterator & itr, SourcePtr & source)
	{
		itr.ConsumeWhitespace();
		source = itr.GetSource();
		if (!itr.ConsumeChar('='))
		{
			return false;
		}
		itr.ConsumeWhitespace();
		virtualKeyword = itr.ConsumeChar('0');
		defaultKeyword = virtualKeyword ? false : itr.ConsumeWord("default");
		deleteKeyword = (virtualKeyword || defaultKeyword)
			? false
		    : itr.ConsumeWord("delete");
		if (! (virtualKeyword || defaultKeyword || deleteKeyword))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.PureVirtual.ExpectedZero"));
		}
		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar(';'))
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.PureVirtual.ExpectedSemicolon"));
		}
		return true;
	}

	// Looks for a number beginning at the iterator exactly.
	// If it sees ANYTHING in the form "1" or "." etc it will advance
	// the iterator.  It returns IMMEDIETELY following the number.
	bool RealNumber(Iterator & itr, double & number)
	{
		Iterator newItr = itr;
		std::stringstream ss;
		bool seenDot = false;
		bool seenDigit = false;
		bool isDigit;
		while(true)
		{
			isDigit = false;
			char c = newItr.Current();
			if (c >= '0' && c <= '9')
			{
				isDigit = true;
				seenDigit = true;
			}
			else if (!seenDot && c == '.')
			{
				seenDot = true;
				isDigit = true;
			}
			else
			{
				break;
			}
			ss << c;
			newItr.Advance(1);
		}
		if (seenDigit)
		{

			ss.seekg(std::ios_base::beg);
			ss >> number;
			itr = newItr;
			return true;
		}
		return false;
	}

	// Very destructive - takes the itr and attempts to consume
	// anything that could be fit in a Scope.
	void ScopeFiller(Iterator & itr)
	{
		// Take whatever you can get, replace the iter
		while (Directives(itr)
				|| Block(itr)
				|| Template(itr)
				|| ConstructorOrDestructor(itr)
				|| FriendDeclaration(itr)
				|| Namespace(itr)
				|| Extern(itr)
				|| Class(itr)
				|| Enum(itr)
				|| Typedef(itr)
				|| VariableOrFunction(itr)
				|| Annotation(itr)
				|| Unit(itr)
				)
		{
		}
	}

	/** If the keyword "signed" or "unsigned" is encountered the length of the
	 *  keyword is returned.
	 *  NOTE: Unlike most methods this doesn't consume whitespace!
	 */
	static int SignedOrUnsignedKeyword(const Iterator & itr)
	{
		Iterator myItr = itr;
		if (myItr.ConsumeWord("signed "))
		{
			return 7;
		}
		else if (myItr.ConsumeWord("unsigned "))
		{
			return 9;
		}
		else
		{
			return 0;
		}
	}

	static int SimpleName(Iterator itr)
	{
		if (itr.Finished() || (!itr.IsAlpha() && !itr.Is('_')))
		{
			return 0;
		}
		int consumed = 1;
		itr.Advance(1);
		while(!itr.Finished() &&
			  (itr.IsAlpha() || itr.IsDigit() || itr.Is("_"))
			  )
		{
			consumed ++;
			itr.Advance(1);
		}
		return consumed;
	}

	static void SimpleNameTests()
	{
		Assert(SimpleName(createTestItr("1")) == 0);
		Assert(SimpleName(createTestItr("_")) == 1);
		Assert(SimpleName(createTestItr("a")) == 1);
		Assert(SimpleName(createTestItr("1a")) == 0);
		Assert(SimpleName(createTestItr("a1")) == 2);
		Assert(SimpleName(createTestItr("_1")) == 2);
		Assert(SimpleName(createTestItr("a_")) == 2);
		Assert(SimpleName(createTestItr("{1")) == 0);
		Assert(SimpleName(createTestItr("a/")) == 1);
		Assert(SimpleName(createTestItr("a/")) == 1);
		Assert(SimpleName(createTestItr("_1:")) == 2);
		Assert(SimpleName(createTestItr("AVeryLongName2_3a:")) == 17);
	}

	bool StaticKeyword(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		return itr.ConsumeWord("static");
	}

	bool Template(Iterator & itr)
	{
		NodePtr templateHome = TemplateParameters(itr);
		if (!templateHome)
		{
			return false;
		}

		this->currentTemplateParameters.push_back(templateHome);

		ConsumeWhitespace(itr);
		if (! (ConstructorOrDestructor(itr, templateHome)
			   || Class(itr, templateHome)
		       || VariableOrFunction(itr, templateHome)))
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Template.ExpectedClassOrFunction"));
		}

		this->currentTemplateParameters.pop_back();
		return true;
	}

	NodePtr TemplateParameters(Iterator & itr)
	{
		ConsumeWhitespace(itr);
		if (!itr.ConsumeWord("template"))
		{
			NodePtr none;
			return none;
		}
		// Comitted.
		NodePtr templateHome = context->CreateFloater("$t");
		TemplateParameterList::Create(
			templateHome,
			Reason::Create(CppAxioms::TemplateParameterListCreation(),
						   itr.GetSource())
		);

		itr.ConsumeWhitespace();
		if (!itr.ConsumeChar('<'))
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Template.ExpectedLT"));
		}
		itr.ConsumeWhitespace();
		TemplateParameterListParse(templateHome, itr, 1);
		return templateHome;
	}

	void TemplateParameterListParse(NodePtr templateHome, Iterator & itr,
		                       int bracketLevel)
	{
		bool seenOne = false;
		while(!itr.ConsumeChar('>'))
		{
			if (seenOne)
			{
				if (!itr.ConsumeChar(','))
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Template.ExpectedComma"));
				}
				itr.ConsumeWhitespace();
			}
			seenOne = true;
			if (TemplateParameterTypeName(templateHome, itr))
			{
				itr.ConsumeWhitespace();
			}
			else
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Template.UnexpectedSyntax"));
			}
		}
	}

	bool TemplateParameterTypeName(NodePtr templateHome, Iterator & itr)
	{
		if (itr.ConsumeWord("typename"))
		{
			itr.ConsumeWhitespace();

			const bool dots = itr.ConsumeWord("...");

			itr.ConsumeWhitespace();

			std::string name;
			if (!ConsumeSimpleName(itr, name))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Template.ExpectedTypenameName"));
			}
			if (templateHome->Find(name))
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Template.NodeNameNotUnique",
								  name));
			}
			NodePtr typeNameNode = templateHome->FindOrCreate(name);
			TemplateTypename::Create(
				typeNameNode, dots,
				Reason::Create(CppAxioms::TemplateTypenameCreation(),
							   itr.GetSource())
			);
			return true;
		}
		else
		{
			return false;
		}
	}

	boost::optional<std::string> TrailingReturnType(Iterator & itr)
	{
		if (!itr.ConsumeWord("->"))
		{
			return boost::none;
		}
		ConsumeWhitespace(itr);
		std::string expr;
		ConsumeExpression(itr, ";{", expr, true);
		boost::trim_right(expr);
		return expr;
	}


	boost::optional<CvQualifier> TypeCvQualifiers(
		Iterator & itr,
		const SimpleTypeModifiers & modifiers)
	{
		Iterator newItr = itr;

		if (!modifiers.Const() && ConstKeyword(newItr)
			&& !(newItr.IsAlpha() || newItr.IsDigit()))
		{
			ConsumeWhitespace(newItr);
			itr = newItr;
			return CvQualifier::Const;
		}
		else if (!modifiers.Mutable() && newItr.ConsumeWord("mutable")
			     && !(newItr.IsAlpha() || newItr.IsDigit()))
		{
			ConsumeWhitespace(newItr);
			itr = newItr;
			return CvQualifier::Mutable;
		}
		else if (!modifiers.Volatile() && newItr.ConsumeWord("volatile")
			     && !(newItr.IsAlpha() || newItr.IsDigit()))
		{
			ConsumeWhitespace(newItr);
			itr = newItr;
			return CvQualifier::Volatile;
		}
		else
		{
			return boost::none;
		}
	}

	/* Grabs the type modifiers, like const or mutable.
	 * Sets the modifiers, returns True if it found anything.
	 * */
	bool SimpleTypeModifiers1(Iterator & itr, SimpleTypeModifiers & modifiers)
	{
		ConsumeWhitespace(itr);
		boost::optional<CvQualifier> cvq;
		bool sawSomething = false;
		while(boost::none != (cvq = TypeCvQualifiers(itr, modifiers)))
		{
			if (modifiers.HasCvQualifier(cvq.get()))
			{
				throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Variable.CvQualifierSeenTwice"));
			}
			modifiers.SetCvQualifier(cvq.get());
			ConsumeWhitespace(itr);
			sawSomething = true;
		}
		return sawSomething;
	}

	/** Just like above, but includes pointers. This can be used after you've
	 *  seen the type. */
	bool SimpleTypeModifiers2(Iterator & itr, SimpleTypeModifiers & modifiers)
	{
		bool sawSomething = SimpleTypeModifiers1(itr, modifiers);

		// Now, we check for a pointer.
		if (itr.ConsumeChar('*'))
		{
			ConsumeWhitespace(itr);
			SimpleTypeModifiers pointerInfo;
			SimpleTypeModifiers2(itr, pointerInfo);
			modifiers.SetPointer(pointerInfo);
			sawSomething = true;
		}

		return sawSomething;
	}

	/** Just like above, except that this sucker can also determine if the
	 *  type is a reference or pointer. So where it is called is different
	 *  from TypeModifiers1. */
	bool TypeModifiers2(Iterator & itr, TypeModifiers & modifiers)
	{
		bool sawSomething = SimpleTypeModifiers1(itr, modifiers);

		// See if this is a "light" reference.
		if (itr.ConsumeWord("~l"))
		{
			modifiers.SetLight(true);
			ConsumeWhitespace(itr);
			sawSomething = true;
		}

		sawSomething = SimpleTypeModifiers2(itr, modifiers);

		// Now, we check for reference.
		if (itr.ConsumeChar('&'))
		{
			modifiers.SetReference(true);
			if (itr.ConsumeChar('&'))
			{
				modifiers.SetRvalueReference(true);
			}
			ConsumeWhitespace(itr);
			sawSomething = true;
		}

		if (itr.ConsumeWord("..."))
		{
			modifiers.SetIsParameterPack(true);
			ConsumeWhitespace(itr);
			sawSomething = true;
		}

		return sawSomething;
	}

	/** Consumes type info into the form of Type, in the format of
	 * [const] [TypeNodeAndArgs] [*] [const] [&]
	 * and sets the new (or, potentially later, found) Type object in "info."
	 * This includes even nested template instances.
	 * Moves itr.
	 */
	bool Type_(Iterator & itr, TypePtr & type)
	{
		Iterator newItr = itr;

		TypeModifiers modifiers;

		bool sawSomething = SimpleTypeModifiers1(newItr, modifiers);

		Iterator beforeTypeInfoItr = newItr;
		if (!(
				DeclTypeAutoHack(newItr, type)
				||
				ConsumeTypeMainNodeAndArguments(newItr, type)
			))
		{
			if (sawSomething)
			{
				// If we saw a CV qualifier, they're committed.
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Variable.CvQualifierMaybeBeforeVar"));
			}
			else
			{
				// Didn't see false and that didn't look like a typename, so
				// we're fine.
				return false;
			}
		}

		// // At this point, we're committed, so start using the actual itr passed
		// // in.
		// if (!mainNode) // The function could not find the node.
 	// 	{
		// 	throw ParserException(beforeTypeInfoItr.GetSource(),
		// 		Messages::Get("CppParser.Variable.UnknownTypeName"));
		// }

		itr = newItr;

		ConsumeWhitespace(itr);

		// Call this again.
		TypeModifiers2(itr, modifiers);
		type->GetModifiers() = modifiers;

		type->Lock();

		return true;
	}

	// looks for "typedef [type]{}" Ignores whitespace.
	// Modifies itr argument if match is found.
	bool Typedef(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);

		AccessPtr access = AccessKeyword(newItr);
		if (*access == *Access::NotSpecified())
		{
			access = currentAccess;
		}
		else
		{
			newItr.ConsumeWhitespace();
		}

		if (!newItr.ConsumeWord("typedef"))
		{
			return false;
		}

		ConsumeWhitespace(newItr);

		TypePtr type;
		if (!Type_(newItr, type))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoTypeDefinitionFound"));
		}

		std::string name;
		if (!ConsumeComplexName(newItr, name))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoName"));
		}

		NodePtr typedefNode = currentScope->FindOrCreate(name);

		TargetPtr tHome = deduceTargetHome(typedefNode);

		TypedefPtr tdp = Typedef::Create(tHome, typedefNode,
			Reason::Create(CppAxioms::TypedefCreation(), newItr.GetSource()),
			*access,
			type);

		ConsumeWhitespace(newItr);

		NodePtr oldScope = currentScope;
		currentScope = typedefNode;

		bool hFile = HFileDirective(newItr);
		if (!hFile && currentTargetIsLibrary())
		{
			// If this thing doesn't have the damn ~hfile keyword then
			// create it's own target, if need be.
			tdp->SwitchOwner(createImplicitUnitForNode(*typedefNode));
		}

		ConsumeWhitespace(newItr);

		while(Directives(newItr) || Annotation(newItr)) {
			ConsumeWhitespace(newItr);
		}
		currentScope = oldScope;

		if (!newItr.ConsumeChar(';'))
		{
			throw ParserException(newItr.GetSource(),
				Messages::Get("CppParser.Typedef.NoSemicolon"));
		}

		itr = newItr; // Success! :)
		return true;
	}

	bool Unit(Iterator & itr)
	{
		Iterator newItr = itr;
		ConsumeWhitespace(newItr);

		if (!newItr.ConsumeWord("~unit"))
		{
			return false;
		}

		// We've seen ~unit. There's no going back!

		ConsumeWhitespace(newItr);

		std::string name;
		std::string defaultRootName;
		if (newItr.Current() == '\"')
		{
			if (!ConsumeStringLiteral(newItr, name))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Unit.NoID1"));
			}
			defaultRootName = name;
		}
		else
		{
			if (!ConsumeSimpleName(newItr, name))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Unit.NoID1"));
			}
			defaultRootName = currentScope->GetPrettyFullName("/") + name;
		}

		bool withBraces = true;
		std::string hFile = defaultRootName + ".h";
		std::string cppFile = defaultRootName + ".cpp";
		std::string type = "lib";

		std::vector<std::string> platforms;
		while(true)
		{
			ConsumeWhitespace(newItr);
			if (newItr.ConsumeWord("hfile"))
			{
				newItr.ConsumeWhitespace();
				if (!newItr.ConsumeChar('='))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.HFileNoEquals.NoID1"));
				}
				newItr.ConsumeWhitespace();
				ConsumeFilePath(newItr, hFile);
			}
			else if (newItr.ConsumeWord("cppfile"))
			{
				newItr.ConsumeWhitespace();
				if (!newItr.ConsumeChar('='))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.CppFileNoEquals.NoID1"));
				}
				newItr.ConsumeWhitespace();
				ConsumeFilePath(newItr, cppFile);
			}
			else if (newItr.ConsumeWord("platform"))
			{
				newItr.ConsumeWhitespace();
				if (!newItr.ConsumeChar('='))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Unit.PlatformNoEquals"));
				}
				newItr.ConsumeWhitespace();
				std::string platform;
				while (ConsumeSimpleName(newItr, platform))
				{
					platforms.push_back(platform);
					newItr.ConsumeWhitespace();
					if (!newItr.ConsumeChar(','))
					{
						break;
					}
					newItr.ConsumeWhitespace();
				}
			}
			else if (newItr.ConsumeWord("type"))
			{
				newItr.ConsumeWhitespace();
				if (!newItr.ConsumeChar('='))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Unit.TypeNoEquals"));
				}
				newItr.ConsumeWhitespace();
				if (!ConsumeSimpleName(newItr, type))
				{
					throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Unit.TypeNoName"));
				}
			}
			else if (newItr.ConsumeChar('{'))
			{
				withBraces = true;
				break;
			}
			else if (newItr.ConsumeChar(';'))
			{
				withBraces = false;
				break;
			}
			else
			{
				throw ParserException(newItr.GetSource(),
						Messages::Get("CppParser.Unit.NoOpeningBrace",
						newItr.GetSource()->GetLine()));
			}
		}

		SourcePtr firstBraceSrc = newItr.GetSource();

		TargetPtr useTarget = currentTarget;
		if (type == "exe" || type == "test" || type == "win32")
		{
			// Create an exe unit.
			std::vector<TargetPtr> deps;
			deps.push_back(currentTarget);
			LibraryTargetPtr lib = boost::dynamic_pointer_cast<LibraryTarget>(currentTarget);
			PathListPtr sources;
			ExeTargetPtr exe = ExeTarget::Create(
				lib,
				name,
				type,
				sources);
			useTarget = exe;
			name = "~unit~" + name;

			// A great question here would be: why did I do this? Why can
			// currentTarget sometimes be used? When would that ever be a
			// good thing? I have a feeling I the code should throw an error
			// if no types match. Way to go, ME.
			BOOST_FOREACH(const std::string & platformName, platforms)
			{
				exe->AddPlatform(platformName);
			}
		}
		// Finally create the new unit target, with the current target
		// as the parent.
		auto_ptr<UnitTarget> utp(UnitTarget::Create(useTarget, true, name));
		utp->SetHFileAsUnknownRelativePath(hFile);
		utp->SetCppFileAsUnknownRelativePath(cppFile);

		TargetPtr oldTarget = currentTarget;
		currentTarget = utp.get();
		utp.release();


		// Now parse everything that is found under this target.
		lastAccess = currentAccess;
		// By default, functions and classes in a unit will be "public,"
		// which just means they'll be found in the header files, which is
		// what most C++ users probably want.
		currentAccess = Access::Public();

		ConsumeWhitespace(newItr);
		ScopeFiller(newItr);
		ConsumeWhitespace(newItr);

		currentAccess = lastAccess;

		if (withBraces)
		{
			if (!newItr.ConsumeChar('}'))
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Unit.NoEndingBrace", firstBraceSrc->GetLine()));
			}
		}
		else
		{
			if (!newItr.Finished())
			{
				throw ParserException(newItr.GetSource(),
					Messages::Get("CppParser.Unit.StatementEof",
					newItr.GetSource()->GetLine()));
			}
		}
		// Switch back to the old target.
		currentTarget = oldTarget;
		itr = newItr; // Success! :)
		return true;
	}

	/** Attempts to parse a variable. Returns false if it can't.  Will move
	 * itr.  If it finds variable type info followed by a complex name it
	 * will return the typeInfo and name.  Otherwise an exception gets thrown.
	 * Does not parse the semicolon though, just stops after the name.
	 * [ WARNING: This name is misleading.  It doesn't fully parse the var,
	 *   just the first part. ]
	 */
	bool Variable(Iterator & itr, AccessPtr & access, bool & _friend,
				  NodePtr & globalHome, bool & isInline, bool & isStatic,
				  bool & isVirtual, bool & isConstExpr,
				  TypePtr & type, std::string & varName)
	{
		using namespace Macaroni::Model::Cpp;

		access = Access::NotSpecified();
		_friend = false;
		bool global = false;
		globalHome.reset();
		isInline = false;
		isConstExpr = false;
		isStatic = false;
		isVirtual = false;
		while(
				(*access == *Access::NotSpecified()
					&& *(access = AccessKeyword(itr)) != *Access::NotSpecified())
				|| (!_friend && (_friend = FriendModifierKeyword(itr)))
				|| (!global && (global = GlobalKeyword(itr, globalHome)))
				|| (!isInline && (isInline = InlineKeyword(itr)))
				|| (!isStatic && (isStatic = StaticKeyword(itr)))
				|| (!isConstExpr && (isConstExpr = ConstExprKeyword(itr)))
				|| (!isVirtual && (isVirtual = VirtualKeyword(itr)))
			  )
		{
			//       ~Monkey
			//  ^..^
			// ~."".
		}

		if (*access == *Access::NotSpecified()) {
			access = currentAccess;
		}

		if (!!global)
		{
			if (*access == *Access::Public() && isStatic)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.PublicGlobalStaticMakesNoSense"));
			}

			// This restricts using the ~global({node}) syntax when the access is
			// hidden, as ~hidden access means the global node gets tucked into the
			// anonymous namespace.
			// Its a sloppy job though so if someone uses the default node path
			// it'll currently work.
			if (*access == *Access::Hidden() && globalHome != currentScope->GetNode())
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.GlobalHomeNodeSpecifierMakesNoSenseWhenHidden"));
			}
		}

		if (!Type_(itr, type))
		{
			return false;
		}
		//if (!VariableType(itr, typeInfo))
		//{
		//	return false;
		//}

		if (itr.ConsumeWord(""))
		// Now we need to see a name.
		if (!ConsumeComplexName(itr, varName))
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.NameExpected"));
		}

		itr.ConsumeWhitespace();

		return true;
	}

	/** Will look at and consume variables with type info, a name, and ending
	 * with a semicolon.  Returns false if nothing is found.
	 * If it finds something it creates the variable within currentScope, and
	 * may define additional nodes if the variable's name is complex. */
	bool VariableOrFunction(Iterator & itr, NodePtr templateHome=NodePtr{})
	{
		using namespace Macaroni::Model::Cpp;

		AccessPtr access = Access::NotSpecified();
		bool _friend;
		NodePtr globalHome;
		bool isInline;
		bool isStatic;
		bool isVirtual;
		bool isConstExpr;
		TypePtr type;
		std::string varName;
		Iterator oldItr = itr; // Save it in case we need to define where the
							   // var definition began.
		if (!Variable(itr, access, _friend, globalHome, isInline, isStatic,
			          isVirtual, isConstExpr, type, varName))
		{
			if (!!globalHome)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.MustFollowGlobalKeyword"));
			}
			return false;
		}

		if (_friend && !globalHome)
		{
			throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.MustFollowGlobalKeyword"));
		}

		if (*access == *Access::NotSpecified())
		{
			access = currentAccess;
		}

		NodePtr node;

		if (!!globalHome)
		{
			node = globalHome->FindOrCreate(varName);
			node->SetAdoptedHome(currentScope);
		}
		else
		{
			node = currentScope->FindOrCreate(varName);
		}

		ConsumeWhitespace(itr);

		std::string initializer;

		if (itr.ConsumeChar('='))
		{
			// Initializer expression following var
			ConsumeExpression(itr, ";", initializer);
		}

		if (itr.ConsumeChar(';'))
		{
			if (templateHome)
			{
				throw ParserException(itr.GetSource(),
					Messages::Get("CppParser.Variable.TemplateNotAllowed"));
			}

			TargetPtr tHome;

			ClassPtr classPtr = currentScope->GetElement<ClassPtr>();

			// Free-standing functions should be linked to a target just like
			// classes.
			if ((globalHome || !classPtr)
				 && !currentScope->GetElement<FunctionOverloadPtr>())
			{
				if (currentTargetIsLibrary())
				{
					throw ParserException(itr.GetSource(), Messages::Get(
						"CppParser.Variable.GlobalVariableMustBeInsideTarget"));
				}
				tHome = deduceTargetHome(currentScope);
			}

			VariablePtr var =
				Variable::Create(tHome, node, access, isStatic, isConstExpr,
				    type, initializer,
					Reason::Create(CppAxioms::VariableScopeCreation(),
					oldItr.GetSource()));


			if (!!globalHome)
			{
				if (!classPtr)
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Global.AllowedForClassesOnly"));
				}
				classPtr->AddGlobal(node);
				if (_friend)
				{
					classPtr->AddFriend(node);
				}
			}
		}
		else if (itr.ConsumeChar('('))
		{
			// Was going to throw exception if initializer found, but really
			// it will parse to ; anyway...
			///*if (!initializer.empty())
			//{
			//	throw ParserException(itr.GetSource(),
			//			Messages::Get("CppParser.Variable.InitializerNotAllowedemicolonExpected"));
			//}*/

			NodePtr oldScope = currentScope;
			NodePtr foNode = node->CreateNextInSequence("Overload#");
			currentScope = foNode;

			if (templateHome)
			{
				// Move the template to their real home...
				context->LandFloater(foNode);
			}

			FunctionArgumentList(itr);

			bool constMember = false;
			bool overrideKeyword = false;
			boost::optional<ExceptionSpecifier> exceptionSpecifier = boost::none;

			while(
				(!constMember && (constMember = ConstKeyword(itr)))
			 || (!overrideKeyword && (overrideKeyword = OverrideKeyword(itr)))
			 || (!exceptionSpecifier
			 	 && (exceptionSpecifier = ParseExceptionSpecifier(itr)))
			){}

			auto trailingReturnType = TrailingReturnType(itr);

			while(Annotation(itr));

			currentScope = oldScope;

			FunctionCodeDefInfo defInfo(*this, itr);

			ReasonPtr fReason = Reason::Create(CppAxioms::FunctionCreation(),
											   oldItr.GetSource());
			FunctionPtr function = Function::Create(node, fReason);

			// If we are in a class, don't give this function a target.
			// Otherwise give it one.
			TargetPtr tHome;
			boost::optional<ImportList> imports = boost::none;
			if (globalHome || (classDepth < 1))
			{
				if (currentTargetIsLibrary())
				{
					throw ParserException(itr.GetSource(), Messages::Get(
						"CppParser.Function.GlobalVariableMustBeInsideTarget"));
				}
				imports = currentImports();
				tHome = deduceTargetHome(currentScope);
			}


			FunctionOverloadPtr fOl =
				FunctionOverload::Create(tHome,
					                     foNode, isInline, access, isStatic,
				                         isVirtual, isConstExpr, type,
										 constMember, overrideKeyword,
										 exceptionSpecifier,
										 fReason, templateHome,
										 trailingReturnType,
										 imports);

			defInfo.ApplyToFunction(fOl);


			if (!!globalHome)
			{
				ClassPtr classPtr = currentScope->GetElement<ClassPtr>();
				if (!classPtr)
				{
					throw ParserException(itr.GetSource(),
						Messages::Get("CppParser.Global.AllowedForClassesOnly"));
				}
				classPtr->AddGlobal(foNode);
				if (_friend)
				{
					classPtr->AddFriend(foNode);
				}
			}

		}
		else
		{
			throw ParserException(itr.GetSource(),
				Messages::Get("CppParser.Variable.SemicolonExpected"));
		}

		return true;
	}

	bool VirtualKeyword(Iterator & itr)
	{
		itr.ConsumeWhitespace();
		return itr.ConsumeWord("virtual");
	}

	bool weAreInsideAClass() const
	{
		// If true, we must be parsing a class.
		return importedNodesStack.size() > 1;
	}

	static void RunTests()
	{
		ComplexNameTests();
		FindNodeTest();
		FindNodeFromImportsTest();
		SimpleNameTests();
	}
};

PippyParser::PippyParser()
{
}

PippyParser::~PippyParser()
{
}

PippyParserPtr PippyParser::Create()
{
	return PippyParserPtr(new PippyParser());
}

int PippyParser::Read(TargetPtr target, Model::SourcePtr source, const std::string & text)
{
	ContextPtr context = target->GetContext();
	CppContext::CreateCppNodes(context);
	ParserFunctions funcs(context, target);
	Iterator itr(text.begin(),
			   text.end(),
			   source);
	try
	{
		funcs.Document(itr);
	}
	catch(ModelInconsistencyException & mie)
	{
		throw ParserException(mie.GetSource(), mie.GetMessage());
	}

	return 1;
}

void PippyParser::RunTests()
{
	Iterator::RunTests();
	ParserFunctions::RunTests();
}

END_NAMESPACE

#endif
