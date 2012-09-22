#define BOOST_TEST_MODULE MyTest
#include <boost/test/unit_test.hpp>

#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <boost/shared_ptr.hpp>

#include <lauxlib.h>
#include <lualib.h>

#include <Macaroni/Tests/Lua/Polo.h>
#include <Macaroni/Tests/Lua/PoloLuaMetaData.h>
#include <Macaroni/Tests/Lua/PoloNameLuaMetaData.h>

using Macaroni::Tests::Lua::Polo;
using Macaroni::Tests::Lua::PoloPtr;
using Macaroni::Tests::Lua::PoloLuaMetaData;

std::string LUA_CODE =
"require 'os';                                                              "
"require 'Macaroni.Tests.Lua.Polo';											"
"require 'Macaroni.Tests.Lua.PoloName';										"
"                                                                           "
"local Polo = Macaroni.Tests.Lua.Polo;										"
"																			"
"function formatTimeToString(date)                                          "
"    return (date.year .. '-' .. date.month .. '-' .. date.day .. '-' ..    "
"            date.hour .. ':' .. date.min .. ':' .. date.sec);              "
"end                                                                        "
"                                                                           "
"function localTime()                                                       "
"    return formatTimeToString( os.date('*t') );                            "
"end                                                                        "
"                                                                           "
"print('This code is being executed from Lua.');                            "
"print('The time is now ' .. localTime() .. '.');                           "
"                                                                           "
"function setMe(polo)                                                       "
"    print([[Polo's name is ]] .. polo:GetName());                          "
"    polo:SetName('Modified in Lua');                                       "
"end	                                                                    "
"                                                                           "
"function setMeViaProperties(polo)                                          "
"    print([[Polo's name is ]] .. polo.Name);                               "
"    polo.Name = polo.Name .. ' again!';                                    "
"end	                                                                    "
"                                                                           "
"function setInvalidProperty(polo)                                          "
"    polo.NotRealPropertyName = 'yo';                                       "
"end	                                                                    "
"                                                                           "
"function callInvalidFunction(polo)                                         "
"    polo:NonLuaFunction('wazzup?');                                        "
"end                                                                        "
"                                                                           "
"function setMeViaStatics(polo)                                             "
"    globalTable = Macaroni.Tests.Lua.Polo;                                 "
"    oldName = globalTable.PointlessStaticFunctionGet(polo);                "
"    newName = oldName .. ' STATICLY.';                                     "
"    globalTable.PointlessStaticFunctionSet(polo, newName);                 "
"end                                                                        "
"                                                                           "
"function setUsingStaticProps(polo)                                         "
"    polo.Name = Macaroni.Tests.Lua.PoloName.Jon.Name                       "
"end                                                                        "
"                                                                           "
"function tryToSetGlobalTable()                                             "
"    Macaroni.Tests.Lua.PoloName.Jon = 'something else';                    "
"end                                                                        "
"                                                                           "
"function triggerTargetMethodException(polo)                                "
"    polo:SetName('Sue');                                                   "
"end                                                                        "
"                                                                           "
"function triggerTargetMethodViaProperty(polo)                              "
"    polo.Name = 'Sue';                                                     "
"end                                                                        "
"                                                                           "
"function printName(polo)                                                   "
"    print(polo.Name)                                                       "
"end                                                                        "
"                                                                           ";

static const struct luaL_Reg libs[] = {
	{"Macaroni.Tests.Lua.Polo", Macaroni::Tests::Lua::PoloLuaMetaData::OpenInLua},
	{"Macaroni.Tests.Lua.PoloName", Macaroni::Tests::Lua::PoloNameLuaMetaData::OpenInLua},
	{0, 0} /* sentinel */
};

void openOurLibs(lua_State * L)
{
	lua_getglobal(L, "package");
	lua_pushstring(L, "preload");
	lua_gettable(L, -2);
	luaL_register(L, 0, libs);
}

BOOST_AUTO_TEST_CASE(MyTestCase)
{
	PoloPtr blah(new Polo());
	blah->SetName("Arthur Mc. Barthur");

	BOOST_CHECK_EQUAL(1, blah->GetReferenceCount());

	lua_State * L = luaL_newstate();
	luaL_openlibs(L);
	openOurLibs(L);
	int error = luaL_loadbuffer(L, LUA_CODE.c_str(), LUA_CODE.size(), "Embedded Code")
		|| lua_pcall(L, 0, 0, 0);
	if (error)
	{
		std::cout << "An error occured within Lua:"
			<< lua_tostring(L, -1) << std::endl;
		BOOST_FAIL("An error occured!");
	}

	// Now, call the function "setMe" which was not run originally, but
	// exists in the LuaState.

	lua_getglobal(L, "setMe");
	PoloLuaMetaData::PutInstanceOnStack(L, blah);
	lua_call(L, 1, 0);

	BOOST_CHECK_EQUAL(true, blah->GetReferenceCount() >= 1
	                        && blah->GetReferenceCount() <= 2);

	BOOST_CHECK_EQUAL("Modified in Lua", blah->GetName());

	{
		// Now set using the property style setter.

		lua_getglobal(L, "setMeViaProperties");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		lua_call(L, 1, 0);

		BOOST_CHECK_EQUAL("Modified in Lua again!", blah->GetName());
	}

	{
		// Now check that we can't set a missing property.
		lua_getglobal(L, "setInvalidProperty");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		bool errorOccured = (lua_pcall(L, 1, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured);
	}

	{
		// Now check that the following function was *not* wrapped, because it had
		// no annotation set.
		lua_getglobal(L, "callInvalidFunction");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		bool errorOccured2 = (lua_pcall(L, 1, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured2);
	}

	{
		// Now do the same thing, only this time via static methods.
		lua_getglobal(L, "setMeViaStatics");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		blah->SetName("Changed somehow");
		lua_call(L, 1, 0);
		BOOST_CHECK_EQUAL("Changed somehow STATICLY.", blah->GetName());
	}

	{
		// This time set it by grabbing the value of a static property.
		lua_getglobal(L, "setUsingStaticProps");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		lua_call(L, 1, 0);
		BOOST_CHECK_EQUAL("Jon", blah->GetName());
	}

	{
		// Check that setting the global table fails.
		lua_getglobal(L, "tryToSetGlobalTable");
		bool errorOccured2 = (lua_pcall(L, 0, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured2);
	}

	{	// Check that if the target method throws an exception, the glue code
		// catches it.
		lua_getglobal(L, "triggerTargetMethodException");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		bool errorOccured = (lua_pcall(L, 1, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured);
	}

	{	// Check that if the target method throws an exception, the glue code
		// for the property setters catches it.
		lua_getglobal(L, "triggerTargetMethodViaProperty");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		bool errorOccured = (lua_pcall(L, 1, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured);
	}

	{	// Check that if the target method throws an exception, the glue code
		// for the property getters catches it..
		blah->SetName("Suzy");
		lua_getglobal(L, "printName");
		PoloLuaMetaData::PutInstanceOnStack(L, blah);
		bool errorOccured = (lua_pcall(L, 1, 0, 0) != 0);
		BOOST_CHECK_EQUAL(true, errorOccured);
	}

	// Finish, and check that the reference count for blah is 1.
	lua_close(L);

	BOOST_CHECK_EQUAL(1, blah->GetReferenceCount());
}
