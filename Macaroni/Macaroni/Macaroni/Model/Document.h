#ifndef MACARONI_DOCUMENT_H
#define MACARONI_DOCUMENT_H

#include "../ME.h"
#include <vector>
#include <string>
#include "Class.h"
#include "Import.h"
#include "Namespace.h"
#include "ParserState.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

class Document;
typedef boost::shared_ptr<Document> DocumentPtr;

/** Represents an MCPP file full of Macaroni code. */
class Document
{
friend class ParserActions;
friend struct DocumentRegister;
public:
	Document(const std::string & name);
	
	~Document();
	
	std::string & GetName();

	inline NamespacePtr GetNamespace(int index){ return namespaces[index]; }
	
	static int OpenInLua(lua_State * L);	

	void Read(const std::string & text);

protected:
	void addNamespace(NamespacePtr ns);

private:	
	void beginParse(const std::string & fileContents);
	
	std::vector<ClassPtr> classes;

	std::vector<std::string> comments;

	std::vector<ImportPtr> imports;

	std::vector<NamespacePtr> namespaces;

	void endParse();
	
	std::string name;

	ParserState parserState;
};

END_NAMESPACE2

#endif
