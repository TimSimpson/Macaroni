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