require "Macaroni.Model.Context";
require "Macaroni.Model.Node";

local Context = Macaroni.Model.Context;
local Node = Macaroni.Model.Node;


Test.register(
{	
name = "Node Tests-Instance Functions",    

tests={	       
 
    ["ParseComplexName creates nothing if given a blank."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultNode, resultName = 
            Node.ParseComplexName(context.RootNamespace, "");
        Test.assert("", resultName);
        Test.assert(0, #context.RootNamespace.Members);
    end,
        
    ["ParseComplexName creates nothing if given a simple name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultNode, resultName = 
            Node.ParseComplexName(context.RootNamespace, "Doggy");
        Test.assert("Doggy", resultName);
        Test.assert(0, #context.RootNamespace.Members);
    end,
    
    ["ParseComplexName creates UnknownScope if given a complex name."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultNode, resultName = 
            Node.ParseComplexName(context.RootNamespace, "Animals::Doggy");
        Test.assert("Doggy", resultName);
        Test.assert(1, #(context.RootNamespace.Members));
        Test.assert("Animals", resultNode.Name);
        Test.assert(context.RootNamespace.Members[1], resultNode);
        Test.assert(0, #(resultNode.Members));
    end,
    
    ["ParseComplexName creates multiple UnknownScope if needed."] = function(this)
        local context = Context.New("{ROOT}", "{WILDCARD}");
        local resultNode, resultName = 
            Node.ParseComplexName(context.RootNamespace, "Organisms::Animals::Kitty");
        Test.assert("Kitty", resultName);
        Test.assert(1, #(context.RootNamespace.Members));
        local organisms = context.RootNamespace.Members[1];
        Test.assert("Organisms", organisms.Name);        
        Test.assert(1, #(organisms.Members));
        Test.assert("Animals", context.RootNamespace.Members[1].Members[1].Name);
        Test.assert(0, #(context.RootNamespace.Members[1].Members[1].Members));
        Test.assert(context.RootNamespace.Members[1].Members[1], resultNode);
        Test.assert("Organisms::Animals", resultNode.FullName);
    end,
    
}

Test.register(
{	
name = "Node Tests-Static Functions",    

tests={	       

    ["IsComplexName returns false for blanks."] = function(this)                
		Test.assert(false, Node.IsComplexName(""));
    end, 
    
    ["IsComplexName returns false for simple names."] = function(this)                
		Test.assert(false, Node.IsComplexName("A"));
		Test.assert(false, Node.IsComplexName("Animal"));
		Test.assert(false, Node.IsComplexName("Doggy42"));
    end, 
    
    ["IsComplexName returns true in the presense of ::."] = function(this)                
		Test.assert(true, Node.IsComplexName("::"));
		Test.assert(true, Node.IsComplexName("A::B"));
		Test.assert(true, Node.IsComplexName("Organisms::Animals::Dog"));
    end, 
    
    ["IsSimpleName returns true for blanks."] = function(this)                
		Test.assert(true, Node.IsSimpleName(""));		
    end, 
    
    ["IsSimpleName returns true for simple names."] = function(this)                
		Test.assert(true, Node.IsSimpleName("A"));		
		Test.assert(true, Node.IsSimpleName("Doggy"));		
		Test.assert(true, Node.IsSimpleName("DogCatDemonHide"));		
    end, 
    
    ["IsSimpleName returns false when faced with ::."] = function(this)                
		Test.assert(false, Node.IsSimpleName("A::B"));		
		Test.assert(false, Node.IsSimpleName("Animals::Doggy"));		
		Test.assert(false, Node.IsSimpleName("Ryu::Enemies::DogCatDemonHide"));		
    end,   
    
    ["SplitFirstNameOffComplexName returns nothing for nothing."] = function(this)        
        local firstPart, lastPart = Node.SplitFirstNameOffComplexName("")        
		Test.assert("", firstPart);
		Test.assert("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns only first part when no :: are found."] = function(this)        
        local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Doggy")        
		Test.assert("Doggy", firstPart);
		Test.assert("", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns simple string when some stuff after :: is found."] = function(this)        
        local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Animal::Doggy")        
		Test.assert("Animal", firstPart);
		Test.assert("Doggy", lastPart);
    end, 
    
    ["SplitFirstNameOffComplexName returns lots of stuff if found after ::."] = function(this)        
        local firstPart, lastPart = Node.SplitFirstNameOffComplexName("Organisms::Animal::Doggy")        
		Test.assert("Organisms", firstPart);
		Test.assert("Animal::Doggy", lastPart);
    end, 
    
    ["SplitNodeAndMemberName scope is blank, member is blank if nothing given."] = function(this)        
        local scope, member = Node.SplitNodeAndMemberName("")
		Test.assert("", scope);
		Test.assert("", member);
    end, 
    
    ["SplitNodeAndMemberName scope is blank if no :: found."] = function(this)        
        local scope, member = Node.SplitNodeAndMemberName("a")
		Test.assert("", scope);
		Test.assert("a", member);
    end,
    
    ["SplitNodeAndMemberName works with one :: found."] = function(this)
        local scopeName, memberName = Node.SplitNodeAndMemberName("b::a")
        Test.assert("b", scopeName);
        Test.assert("a", memberName);
    end,
    
    ["SplitNodeAndMemberName works with two :: found."] = function(this)
        local scopeName, memberName = Node.SplitNodeAndMemberName("Arg::big::Orange")
        Test.assert("Arg::big", scopeName);
        Test.assert("Orange", memberName);
    end,
    
    ["SplitNodeAndMemberName works with three (or lets presume more) :: found."] = function(this)
        local scopeName, memberName = Node.SplitNodeAndMemberName("King::Darius::Gradius::Vic")
        Test.assert("King::Darius::Gradius", scopeName);
        Test.assert("Vic", memberName);
    end,   
    
    ["SplitComplexName returns zero elements for blank."] = function(this)
        local names = Node.SplitComplexName("")
        Test.assert({}, names);        
    end,   
    
    ["SplitComplexName returns one element for single names."] = function(this)
        local names = Node.SplitComplexName("Porcupine")
        Test.assert({"Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 2 elements when it needs to."] = function(this)
        local names = Node.SplitComplexName("Animals::Porcupine")
        Test.assert({"Animals", "Porcupine"}, names);        
    end,   
    
    ["SplitComplexName returns 3 or more elements when it needs to."] = function(this)
        local names = Node.SplitComplexName("Organisms::Animals::Porcupine")
        Test.assert({"Organisms", "Animals", "Porcupine"}, names);        
    end,   
}

} -- End of Test table.
); -- End of register call
