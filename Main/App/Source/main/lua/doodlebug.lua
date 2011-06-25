--------------------------------------------------------------------------------
-- Copyright 2011 Tim Simpson
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--    http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--------------------------------------------------------------------------------

require "Macaroni.Model.File";
require "Macaroni.Model.Namespace";
print("Hello from Lua.");

print(require);
print("PACKAGE.PATH:");
print(package.path);
print("");
print("");
---package.path= "?";
require "Macaroni/Model/FileTests";

print(Macaroni.Model.FileTests);

print("DID IT PRINT?");

file = Macaroni.Model.File.New("TestData/simple.mcpp");

print("hi?");

print("More zany action below:");

namespaces = file.Namespaces; --Macaroni.Model.Namespace.CreateRoot("{LuaRoot}");

print("Printing file.Namespaces=");
print(namespaces );

namespace0 = namespaces[0];
print(namespace0);

--print ("Name is " .. ns.Name);

--[[
Object Table

MetaTable-
   - __Index table
       - __Index table's metatable (with __index overwritten)



Each module is stored as something with Macaroni prefix, like "Macaroni_Model_Class".
Modules relate to C++ objects, such as Macaroni.Model.Class, which matches where
their C++ files are.
Macaroni.File

Macaroni = {};

file = Macaroni.File.New("file.mcpp");

file.Classes = { Macaroni_Class[] }
file.Namespaces = { Macaroni.Model.Namespaces[] }

Macaroni.Class;
class.Name = "NameOfClass"
class.Summary = "Summary text."
class.Fields = { Macaroni.Field[] }
class.Methods = { Macaroni.Method[] }
class.Properties = { Macaroni.Property[] }
class.Tests = { Macaroni.Test[] }

Macaroni.Field
field.Name = "NameOfField";

--]]