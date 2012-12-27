require "Macaroni.Model.AnnotationTable";
require "Macaroni.Model.AnnotationValue";
require "Macaroni.Model.Context";
require "Macaroni.IO.GeneratedFileWriter";
require "Macaroni.Model.Library";
require "Macaroni.Model.Node";
require "Macaroni.IO.Path";

print("This reads Annotations!");

function AssertEqual(msg, a, b)
	print(msg .. tostring(a))
	if a ~= b then
		error(tostring(a) .. " not equal to " .. tostring(b));
	end
end

function Generate(library, path)
    local context = library.Context;
    local root = context.Root;
    local employee = root:Find("Reosui::Database::Employee");    
    local attr = employee.Annotations;
    print(root.FullName);    
    local simple = attr["FakeORM::IsSimple"];
    print("Simple=" .. tostring(simple));
    AssertEqual("Simple Value as Bool = ", simple.ValueAsBool, false);
    --print("Simple Value as Bool = " .. tostring(simple.ValueAsBool));
     
    print("MaxInstances=" .. tostring(attr["FakeORM::MaxInstances"]));
    --AssertEqual("MaxInstances=", attr["FakeORM::MaxInstances"]);
--    print("MaxInstances Value as Number =" .. tostring(attr["FakeORM::MaxInstances"].ValueAsNumber));
    AssertEqual("MaxInstances Value as Number =", 
    	attr["FakeORM::MaxInstances"].ValueAsNumber, 20);
    
    print("Partner=" .. tostring(attr["FakeORM::Partner"]));
    --print("Partner Value as Node =" .. tostring(attr["FakeORM::Partner"].ValueAsNode));
    AssertEqual("Partner Value as Node =", 
    	attr["FakeORM::Partner"].ValueAsNode, root:Find("Reosui::Database::Dog"));
    
    print("Partner2=" .. tostring(attr["FakeORM::Partner2"]));
    --print("Partner2 Value as Node =" .. tostring(attr["FakeORM::Partner2"].ValueAsNode));
    AssertEqual("Partner2 Value as Node =", 
    	attr["FakeORM::Partner2"].ValueAsNode, root:Find("Reosui::Database::Dog"));
    
    print("CodeBlock=" .. tostring(attr["FakeORM::CodeBlock"]));
    --print("CodeBlock Value as String =" .. tostring(attr["FakeORM::CodeBlock"].ValueAsString));
    AssertEqual("CodeBlock Value as String =",
    	attr["FakeORM::CodeBlock"].ValueAsString, " Look, arbitrary! ");
    
    print("PersonalizedGreeting=" .. tostring(attr["FakeORM::PersonalizedGreeting"]));
    -- print("PersonalizedGreeting Value as String =" .. tostring(attr["FakeORM::PersonalizedGreeting"].ValueAsString));
    AssertEqual("PersonalizedGreeting Value as String =", attr["FakeORM::PersonalizedGreeting"].ValueAsString, "blah blah blah");
    
    print("Sleep=" .. tostring(attr["FakeORM::Sleep"]));
    --print("Sleep Value as Table =" .. tostring(attr["FakeORM::Sleep"].ValueAsTable)); 
    AssertEqual("Sleep Value as Table . often =", 
    	attr["FakeORM::Sleep"].ValueAsTable.often.ValueAsString, 
    	"yes"); 
	AssertEqual("Sleep Value as Table . awayMessage =", 
    	attr["FakeORM::Sleep"].ValueAsTable.awayMessage.ValueAsString, 
    	"Please... let me sleep... a little more... please..."); 
    	    
    local someMethod = employee:Find("SomeMethod").Children[1]
    local testAttr = someMethod.Annotations["FakeTestLib::Test"];
    AssertEqual("Method attr test .enabled = ",
    			testAttr.ValueAsTable.enabled.ValueAsBool, true)
    AssertEqual("Method attr test .  = ",
    			testAttr.ValueAsTable.priority.ValueAsNumber, 25)
    			
    			
    -- Make sure if we read the wrong thing nothing goes haywire
    local status, err = pcall(function() 
    	return testAttr.ValueAsTable.priority.ValueAsString
    end);
    AssertEqual("Should fail if accessing attr diffrently!" , status, false);
    -- AssertEqual("Should fail if accessing attr diffrently!" , err, 10);
    
end
