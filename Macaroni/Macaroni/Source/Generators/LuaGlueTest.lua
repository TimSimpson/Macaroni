require "LuaGlue"
require "Macaroni.Parser.Pippy.PippyParser";
require "Macaroni.Parser.Parser";
require "Macaroni.Parser.ParserException";

local PippyParser = Macaroni.Parser.Pippy.PippyParser;

local function mixinContext(self, sourceCode)
    self.context = Context.New("{ROOT}");    
    self.library = self.context:CreateLibrary("TypeTests", "1.5");
    self.generator = LuaGlueGenerator.new(self.context.Root);		    
	local cppPrimitives = self.generator.RootNode:FindOrCreate("{C++ Primitives}");
	self.intNode = cppPrimitives:FindOrCreate("int");
	self.boolNode = cppPrimitives:FindOrCreate("bool");    
    
    
    local parser = PippyParser.Create();         
    local file = FileName.Create("LuaGlueTest Made Up File.mcpp");           
    local root = self.context.Root;
    local src = Source.Create(file, 1, 1);
        
    self.trim = function(s)
	  return (s:gsub("^%s*", ""))
	end;
    
    parser:Read(self.library, src, sourceCode);    
end

Test.register(
{
name = "LuaGlueTests",
tests =
{
	{
		name = "Type Manipulators Test",
		init = function(self)
			mixinContext(self, [[
				~import Macaroni::Lua::LuaClass; //<-- The Attribute to use
				~import Example::Polo;
				~import Example::PoloPtr;
				
				namespace Example 
				{	
					// Plain Old Lua Object, get it?
					class Polo
					{
						@LuaClass 
						[
							ReferenceType = PoloPtr,
						]
					};
					
					class PoloPtr
					{
					};
				}
			]]);						
					
			self.polo = self.generator.RootNode:Find("Example::Polo");
			self.poloPtr = self.generator.RootNode:Find("Example::PoloPtr");
			self.stdstring = self.generator.RootNode:FindOrCreate("std::string");
			self.stdstring = self.generator.RootNode:FindOrCreate("std::string");
			self.oldLuaWrapperArguments = self.generator.LuaWrapperArguments;
			--[[self.generator.LuaWrapperArguments = function(node)
				log "I got called.";
				if (node == self.polo) then
					return {
						node = node;
						referenceType = self.poloPtr;
						glueType = self.context.Root:FindOrCreate("PoloLuaMetaData");
					};
				else
					error("NOPE!", 2);
				end
			end;]]--
		end,
		tests = {
			["Polo"] = function(self)
				local type = Type.New(self.polo, {});
				local tm = self.generator:TypeManipulators(type)
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim([[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
				NEW_LINE .. [[Example::Polo & blah = *(*(blah_AsRef));]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["const Polo"] = function(self)
				local type = Type.New(self.polo, { Const = true });
				local tm = self.generator:TypeManipulators(type);
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim(
				[[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
				NEW_LINE .. [[const Example::Polo & blah = *(*(blah_AsRef));]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["const Polo *"] = function(self)
				local type = Type.New(self.polo, { Const = true, Pointer = true });
				local tm = self.generator:TypeManipulators(type)
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim([[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
NEW_LINE .. 				
[[const Example::Polo * blah = *(blah_AsRef);]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["Polo &"] = function(self)
				local type = Type.New(self.polo, { Reference = true });
				local tm = self.generator:TypeManipulators(type)
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim([[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
NEW_LINE .. [[Example::Polo & blah = *(*(blah_AsRef));]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["const Polo &"] = function(self)
				local type = Type.New(self.polo, { Const = true, Reference = true });
				local tm = self.generator:TypeManipulators(type)
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim([[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
NEW_LINE .. [[const Example::Polo & blah = *(*(blah_AsRef));]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["const Polo * const"] = function(self)
				local type = Type.New(self.polo, { Const = true, ConstPointer = true, Pointer = true });
				local tm = self.generator:TypeManipulators(type)
				Test.assert([[Example::PoloPtr & blah = Example::PoloLuaMetaData::GetInstance(L, 5);]], tm.get("blah", 5));
				Test.assert(self.trim([[Example::PoloPtr & blah_AsRef = Example::PoloLuaMetaData::GetInstance(L, 5);]] ..
NEW_LINE .. [[const Example::Polo * const blah = *(blah_AsRef);]]), tm.convertArgument("blah", 5));
				Test.assert([[Example::PoloLuaMetaData::PutInstanceOnStack(L, blah);]], tm.put("blah"));
			end, 
			["int"] = function(self)				
				local type = Type.New(self.intNode, {});
				local tm = self.generator:TypeManipulators(type)			
				Test.assert(tm.get("blah", 5), [[int blah(luaL_checkint(L, 5));]]);
				Test.assert([[int blah(luaL_checkint(L, 5));]], tm.convertArgument("blah", 5));
				Test.assert(tm.put("blah"), [[lua_pushint(L, blah);]]);
			end,   
			["const int"] = function(self)				
				local type = Type.New(self.intNode, { Const = true });
				local tm = self.generator:TypeManipulators(type)			
				Test.assert(tm.get("blah", 5), [[int blah(luaL_checkint(L, 5));]]);
				Test.assert([[int blah(luaL_checkint(L, 5));]], tm.convertArgument("blah", 5));
				Test.assert(tm.put("blah"), [[lua_pushint(L, blah);]]);
			end,    
			["std::string"] = function(self)
				local type = Type.New(self.stdstring, {});
				local tm = self.generator:TypeManipulators(type)			
				Test.assert([[const std::string blah(luaL_checkstring(L, 5));]], tm.get("blah", 5));
				Test.assert([[const std::string blah(luaL_checkstring(L, 5));]], tm.convertArgument("blah", 5));
				Test.assert(tm.put("blah"), [[luaL_pushstring(L, blah.c_str());]]);
			end,    
			["undo test context"] = function(self)
				self.generator.LuaWrapperArguments = self.oldLuaWrapperArguments;
			end,        
		}        
	}, -- end of TypeManipulatorTests
	
	{
		name = "Wrap Methods Test",
		init = function(self)
			mixinContext(self, [[
				~import Macaroni::Lua::LuaClass; //<-- The Attribute to use
				~import Example::Action;
				~import Example::ActionPtr;
				~import Example::Polo;
				~import Example::PoloPtr;
				~import std::string;
				
				namespace Example 
				{	
					class Action
					{
						@LuaClass
						[ ReferenceType = ActionPtr ]
					};
					class ActionPtr{};
					
					// Plain Old Lua Object, get it?
					class Polo
					{
						@LuaClass 
						[
							ReferenceType = PoloPtr,
						]
												
						public const string & DoSomething(const int index, const std::string & name, 
							const Action & action) const
						{
							return name;
						}
					};
					
					class PoloPtr
					{
					};
				}
			]]);						
					
			self.polo = self.generator.RootNode:Find("Example::Polo");
			self.poloPtr = self.generator.RootNode:Find("Example::PoloPtr");
			self.poloDoSomething = self.polo:Find("DoSomething");
			self.stdstring = self.generator.RootNode:FindOrCreate("std::string");
			self.stdstring = self.generator.RootNode:FindOrCreate("std::string");
			self.oldLuaWrapperArguments = self.generator.LuaWrapperArguments;			
		end,
		tests = {
			["wrap a function"] = function(self)
				local type = Type.New(self.polo, {});
				local tm = self.generator:TypeManipulators(type)
				local method = self.generator:wrapMethod(self.poloDoSomething);
				Test.assert(self.trim("\n\tint DoSomething(lua_State * L)" ..
					"\n\t{" ..
					"\n\t\tExample::PoloPtr & instance = Example::PoloLuaMetaData::GetInstance(L, 1);" ..
					"\n\t\tint arg1(luaL_checkint(L, 2));" ..
					"\n\t\tconst std::string arg2(luaL_checkstring(L, 3));" ..
					"\n\t\tExample::ActionPtr & arg3_AsRef = Example::ActionLuaMetaData::GetInstance(L, 4);" ..
					NEW_LINE .. "const Example::Action & arg3 = *(*(arg3_AsRef));" ..
					"\n\t\tconst std::string &  rtn = instance->DoSomething(arg1, arg2, arg3);" ..
					"\n\t\tluaL_pushstring(L, rtn.c_str());" ..
					"\n\t\treturn 1;" ..
					"\n\t}"),
				method.text);
			end, 			
		}        
	}, -- end of wrapMethods test
} -- end of tests
} -- end of table
); -- end of register call
