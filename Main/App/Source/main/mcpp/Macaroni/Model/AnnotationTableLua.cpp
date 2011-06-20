#ifndef MACARONI_MODEL_ATTRIBUTETABLELUA_CPP
#define MACARONI_MODEL_ATTRIBUTETABLELUA_CPP

#include "AnnotationTable.h"
#include "AnnotationValue.h"
#include "AnnotationTableLua.h"
#include "AnnotationValueLua.h"
#include "Node.h"
#include "NodeLua.h"
#include <Macaroni/Model/ReasonLuaMetaData.h>
#include <sstream>

#define LUAGLUE_STARTNAMESPACE BEGIN_NAMESPACE2(Macaroni, Model)
#define LUAGLUE_ENDNAMESPACE	END_NAMESPACE2
#define LUAGLUE_CLASSNAME AnnotationTable
#define LUAGLUE_CLASSREFNAME AnnotationTablePtr
#define LUAGLUE_CLASSFULLLUANAME "Macaroni.Model.AnnotationTable"
#define LUAGLUE_CLASSFULLCPPNAME Macaroni::Model::AnnotationTable
#define LUAGLUE_REGISTRATIONCLASSNAME AnnotationTableLuaMetaData
#define LUAGLUE_OPENOTHERMODULES /** F'no */
#define LUAGLUE_CREATEMETATABLE YESPLEASE
#define LUAGLUE_HELPERCLASS				AnnotationTableLuaGlueHelperClass

#include "../LuaGlue.hpp"

#define TRY try {
#define CATCH } catch(const std::exception & ex){ return luaL_error(L, ex.what()); }


	static int __index(lua_State * L, const AnnotationTablePtr & ptr, 
									  const std::string & index)
	{	
		TRY
			AnnotationValuePtr value = ptr->GetByName(index);
			if (!!value)
			{
				AnnotationValueLuaMetaData::PutInstanceOnStack(L, value);
				return 1;
			}		
			
			// Currently only Strings allowed here due to how I this Macro
			// madness works. I'll change that later...

			lua_pushnil(L);			
			return 1;
		CATCH
	}

	static int __tostring(lua_State * L)
	{
		TRY
			AnnotationTablePtr & ptr = getInstance(L);
			std::stringstream ss;
			ss << "[AnnotationTable, Home Node=";
			ss << ptr->GetHomeNode()->GetFullName();
			ss << "]";
			lua_pushstring(L, ss.str().c_str());
			return 1;
		CATCH
	}	

	#define LUAGLUE_ADDITIONALMETATABLEMETHODS \
		{"__tostring", LUAGLUE_HELPERCLASS::__tostring}, 

	#define LUAGLUE_ADDITIONALTABLEMETHODS \
		/*{"LuaCreate", LUAGLUE_HELPERCLASS::LuaCreate},*/

#include "../LuaGlue2.hpp"

#endif