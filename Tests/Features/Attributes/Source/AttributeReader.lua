require "Macaroni.Model.AttributeTable";
require "Macaroni.Model.AttributeValue";
require "Macaroni.Model.Context";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.Model.Node";
require "Macaroni.IO.Path";

print("This reads Attributes!");

function Generate(library, path)
    local context = library.Context;
    local root = context.Root;
    local employee = root:Find("Reosui::Database::Employee");    
    local attr = employee.Attributes;
    print(root.FullName);    
    local simple = attr["FakeORM::IsSimple"];
    print("Simple=" .. tostring(simple));
    print("Simple Value as Bool = " .. tostring(simple.ValueAsBool));
     
    print("MaxInstances=" .. tostring(attr["FakeORM::MaxInstances"]));
    print("MaxInstances Value as Number =" .. tostring(attr["FakeORM::MaxInstances"].ValueAsNumber));
    
    print("Partner=" .. tostring(attr["FakeORM::Partner"]));
    print("Partner Value as Node =" .. tostring(attr["FakeORM::Partner"].ValueAsNode));
    print("Partner2=" .. tostring(attr["FakeORM::Partner2"]));
    print("Partner2 Value as Node =" .. tostring(attr["FakeORM::Partner2"].ValueAsNode));
    
    print("CodeBlock=" .. tostring(attr["FakeORM::CodeBlock"]));
    print("CodeBlock Value as String =" .. tostring(attr["FakeORM::CodeBlock"].ValueAsString));
    
    print("PersonalizedGreeting=" .. tostring(attr["FakeORM::PersonalizedGreeting"]));
    print("PersonalizedGreeting Value as String =" .. tostring(attr["FakeORM::PersonalizedGreeting"].ValueAsString));
    
    print("Sleep=" .. tostring(attr["FakeORM::Sleep"]));
    print("Sleep Value as Table =" .. tostring(attr["FakeORM::Sleep"].ValueAsTable)); 

    print("Sleep.often=" .. tostring(attr["FakeORM::Sleep"].ValueAsTable.often));
    print("Sleep.often value as String=" .. attr["FakeORM::Sleep"].ValueAsTable.often.ValueAsString);
end
