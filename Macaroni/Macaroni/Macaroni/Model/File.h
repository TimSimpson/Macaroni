#ifndef MACARONI_FILE_H
#define MACARONI_FILE_H

#include "../ME.h"
#include <vector>
#include <string>
#include "Class.h"
#include "Import.h"
#include "Namespace.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

class File;
typedef boost::shared_ptr<File> FilePtr;

/** Represents an MCPP file full of Macaroni code. */
class File
{
friend class ParserActions;
friend struct FileRegister;
public:
	File(std::string & name);
	~File();
	std::string & GetName();
	inline NamespacePtr GetNamespace(int index){ return namespaces[index]; }
	static int OpenInLua(lua_State * L);	
private:	
	void beginParse(std::string & fileContents);
	
	std::vector<ClassPtr> classes;

	std::vector<std::string> comments;

	std::vector<ImportPtr> imports;

	std::vector<NamespacePtr> namespaces;

	void endParse();
	
	std::string name;
};

END_NAMESPACE2

#endif
