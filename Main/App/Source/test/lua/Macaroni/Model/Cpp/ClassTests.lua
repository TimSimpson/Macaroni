require "Macaroni.Model.Cpp.Access";
require "Macaroni.Model.Axiom";
require "Macaroni.Model.Cpp.Class";
require "Macaroni.Model.Context";
require "Macaroni.Model.FileName";
require "Macaroni.Model.Library";
require "Macaroni.Model.Node";
require "Macaroni.Model.NodeList";
require "Macaroni.Model.Reason";
require "Macaroni.Model.Source";

local Access = Macaroni.Model.Cpp.Access;
local Axiom = Macaroni.Model.Axiom;
local Class = Macaroni.Model.Cpp.Class;
local Context = Macaroni.Model.Context;
local FileName = Macaroni.Model.FileName;
local Library = Macaroni.Model.Library;
local Node = Macaroni.Model.Node;
local NodeList = Macaroni.Model.NodeList;
local Reason = Macaroni.Model.Reason;
local Source = Macaroni.Model.Source;

Test.register(
{	
name = "Class Tests",    
tests = {    
    {   name = "Creating a class by hand",
        init = function(this)
            this.context = Context.New("{r}");
            this.library = this.context.RootLibrary;
            this.root = this.context.Root;
            this.newNode = this.root:Find("Dog");
            Test.assert(nil, this.newNode);
            this.newNode = this.root:FindOrCreate("Dog");
            Test.assertFalse(nil, this.newNode);
            this.imports = NodeList.New{};
            this.reason = nil;
            this.axiom = Axiom.LuaCreate("LuaGlue.LuaClassWraps");
            this.fileName = FileName.Create("MadeUpFile");
            this.src = Source.Create(this.fileName, 3, 5);
            this.reason = Reason.Create(this.axiom, this.src);
            this.newClass = Class.Create(this.library, this.newNode, this.imports, this.reason);
        end,
        tests = {
            ["Find returns the same Node that was created."] = function(this)
                local foundNode = this.root:Find("Dog");
                Test.assert(this.newNode, foundNode);
            end,
            ["The Children property has the same reference."] = function(this)
                local nodes = this.root.Children;
                Test.assert(1, #nodes);
                Test.assert(foundNode, nodes[0]);
            end,
            ["The Class can use Member methods such as Library."] = function(this)
                local class = this.newClass;
                local library = class.Library;
                Test.assert(this.library, library);
            end,
            ["The Class can use Member methods such as TypeName."] = function(this)
                local class = this.newClass;
                local typename = class.TypeName;
                Test.assert("Class", tostring(typename));
            end,
            ["The Class can use Class methods such as ImportedNodes."] = function(this)
                local class = this.newClass;
                local importedNodes = class.ImportedNodes;
                Test.assert(0, #importedNodes);
            end,
            --["The default of a node is private."] = function(this)
            --    local foundNode = this.root:Find("Dog");
            --    local expected = Access.Private;
            --    local actual = foundNode.Access;
            --    Test.assert(expected, actual); 
            --end,
        }        
    },   -- end of test 1
} -- tests
}); -- End of register call
